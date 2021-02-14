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