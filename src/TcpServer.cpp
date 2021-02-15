#include "TcpServer.h"
#include <cassert>
#include <cstdio>
#include <cstring>


TcpThreadPool::TcpThreadPool(size_t threadCount,  SocketFormat_fn_ptr handlerFn): _threadCount(threadCount),
                                                  _threads(new std::thread[_threadCount]),
                                                  _shuttingDown(false),
                                                  _handlerFn(handlerFn)
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

bool  TcpThreadPool::handle(tcp_socket_t sd)
{
	return (bool)_handlerFn(sd);
}

void  TcpThreadPool::work()
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
{
	    std::memset(&_address, 0, sizeof(_address));
	    _address.sin_family = AF_INET;
	    _address.sin_addr.s_addr = INADDR_ANY;
	    _address.sin_port = htons(_config.port);
}

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

TcpServerThread::TcpServerThread(const TcpServerConfig& config): _shuttingDown(false), 
                                                                 _server(config)
{}

void TcpServerThread::go()
{
	_thread = std::thread([this]{
		if(!this->_server.go())
			this->stop();
	});
	if (_shuttingDown.load() && _thread.joinable()) {
		_thread.join();
	}
}

void TcpServerThread::stop()
{
	_shuttingDown.store(true);
	_server.stop();
}

void TcpServerThread::join()
{
	if (_thread.joinable())
		_thread.join();
}

