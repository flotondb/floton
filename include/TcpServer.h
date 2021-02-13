#ifndef FLOTON_TCP_SERVER_H
#define FLOTON_TCP_SERVER_H

#include <thread>
#include <mutex>

#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
#    undef UNICODE
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
#else // _WIN32
#    include <unistd.h> 
#    include <sys/socket.h> 
#    include <netinet/in.h> 
#endif // !_WIN#2


class TcpServer {
public:
	TcpServer(unsigned short port): _port(port)
	{}
private:
    unsigned short _port;
    int _back_log;
    int _server_fd;
    sockaddr_in _address;
};

#endif // FLOTON_TCP_SERVER_H