#ifndef FLOTON_TCP_SERVER_H
#define FLOTON_TCP_SERVER_H

#include <thread>
#include <mutex>
#include <condition_variable>

#ifdef _WIN32
#    pragma comment (lib, "Ws2_32.lib")
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
#else // _WIN32
#    include <unistd.h> 
#    include <sys/socket.h> 
#    include <netinet/in.h> 
#endif // !_WIN32


class TcpConnection {
public:
	TcpConnection(int fd);
	~TcpConnection();
private:
	int _fd;
};


struct TcpServerConfig {
	unsigned short port;
	int backLogCount;
	bool keepAlive;
};


class TcpServer {
public:
	TcpServer(const TcpServerConfig& config);
	bool go();
private:
    TcpServerConfig _config;
    int _server_fd;
    sockaddr_in _address;
};

#endif // FLOTON_TCP_SERVER_H