#ifndef FLOTON_TCP_SERVER_H
#define FLOTON_TCP_SERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "SocketFormat.h"



class TcpThreadPool {
public:
	TcpThreadPool(size_t threadCount);
	~TcpThreadPool();

	void go();
	void stop();
	bool addConnection(tcp_socket_t sd);
private:
	void work();
	bool handle(tcp_socket_t sd);

	size_t _threadCount;
	std::thread* _threads;
	std::atomic<bool> _shuttingDown;
	std::queue<tcp_socket_t> _conns;
	std::condition_variable _cond;
	mutable std::mutex _mut;

	friend class TcpServer;
};


struct TcpServerConfig {
	unsigned short port;
	int backLogCount;
	size_t threadPoolSize;
};


class TcpServer {
public:
	TcpServer(const TcpServerConfig& config);
	bool go();
	void stop();
private:
    TcpServerConfig _config;
    TcpThreadPool _tpool;
    std::atomic<bool> _shuttingDown;
    int _server_fd;
    sockaddr_in _address;
};

#endif // FLOTON_TCP_SERVER_H