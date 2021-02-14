#include "TcpServer.h"
#include <cassert>

TcpThreadPool::TcpThreadPool(size_t threadCount): _threadCount(threadCount),
                                                  _threads(new std::thread[_threadCount]),
                                                  _shuttingDown(false)
                                                  {}

TcpThreadPool::~TcpThreadPool()
{
	assert(_shuttingDown.load());
	delete[] _threads;
}

bool TcpThreadPool::addConnection(int sd)
{
	if (_shuttingDown.load())
		return false;
	{
		std::unique_lock<std::mutex> lock(_mut);
		_conns.push(sd);
	}
	_cond.notify_one();
	return true;
}

bool handle(tcp_socket_t sd)
{
	// to do
	return true;
}

void work()
{
	while (true) {
		std::unique_lock<std::mutex> lock(_mut);
		_cond.wait(lock, [&]{return !_conns.empty() || _shuttingDown.load();});
		// handle notify_all
		if (_conns.empty() || _shuttingDown.load()) {
			break;
		 }
		 tcp_socket_t got = _conns.front();
		 _conns.pop();
		 handle(got);
	}
}

void TcpThreadPool::go()
{
	for (int i = 0; i < _threadCount; ++i)
	{
		_threads[i] = std::thread([this]{
			this->work();
		});
	}
}

void TcpThreadPool::stop()
{
	_shuttingDown.store(true);
	_cond.notify_all();
	for (int i = 0; i < _threadCount; ++i)
	{
		_threads[i].join();
	}
}


TcpServer::TcpServer(const TcpServerConfig& config): _config(config),
                                                     _tpool(config.threadPoolSize),
                                                     _shuttingDown(false)
{}

void TcpServer::stop()
{
	_shuttingDown.store(true);
	_tpool.stop();
}

bool TcpServer::go()
{
	    if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	    	// error
	        return false;
	    }

	    if (bind(_server_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
	    	// error
	        return false;
	    }

	    if (listen(_server_fd, _config.backLogCount) < 0) {
	    	// error
	        return false;
	    }
	    int address_length = sizeof(_address);
	    _tpool.go();
	    // Start loop for listening to accept calls
	    while(!_shuttingDown.load()) {
	        int new_socket = 0;
	        if ((new_socket = accept(_server_fd, (struct sockaddr *)&_address,  
	                       (socklen_t*)&address_length))<0) {

	            return false;
	        }
	        _tpool.addConnection(new_socket);
	    }
	    
	return true;
}

