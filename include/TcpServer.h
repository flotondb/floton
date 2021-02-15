#ifndef FLOTON_TCP_SERVER_H
#define FLOTON_TCP_SERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "SocketFormat.h"


class TcpThreadPool {
public:
	TcpThreadPool(size_t threadCount, SocketFormat_fn_ptr handlerFn = SocketFormat_echo_text);
	~TcpThreadPool();

	void go();
	void stop();
	bool addConnection(tcp_socket_t sd);

	bool isShuttingDown() const { return _shuttingDown.load(); }
private:
	void work();
	bool handle(tcp_socket_t sd);

	size_t _threadCount;
	std::thread* _threads;
	std::atomic<bool> _shuttingDown;
	SocketFormat_fn_ptr _handlerFn;
	std::queue<tcp_socket_t> _conns;
	std::condition_variable _cond;
	mutable std::mutex _mut;
};


struct TcpServerConfig {
	unsigned short port;
	int backLogCount;
	size_t threadPoolSize;
	SocketFormat_fn_ptr handlerFn;
};

class TcpServer {
public:
	TcpServer(const TcpServerConfig& config);
	bool go();
	void stop();

	bool isShuttingDown() const { return _shuttingDown.load(); }
private:
    TcpServerConfig _config;
    TcpThreadPool _tpool;
    std::atomic<bool> _shuttingDown;
    int _server_fd;
    sockaddr_in _address;
};

class TcpServerThread {
public:
	TcpServerThread(const TcpServerConfig& config);
	void go();
	void stop();
	void join();

	bool isShuttingDown() const { return _shuttingDown.load(); }
private:
	std::atomic<bool> _shuttingDown;
	TcpServer _server;
	std::thread _thread;
};

#endif // FLOTON_TCP_SERVER_H