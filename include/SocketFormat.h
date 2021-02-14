#ifndef FLOTON_SOCKET_FORMAT_H
#define FLOTON_SOCKET_FORMAT_H

#ifdef _WIN32
#    pragma comment (lib, "Ws2_32.lib")
#    pragma comment (lib, "Mswsock.lib")
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    include <winsock2.h>
#    include <ws2tcpip.h>
#else // _WIN32
#    include <unistd.h> 
#    include <sys/socket.h> 
#    include <netinet/in.h> 
#endif // !_WIN32

// Socket type defines
#ifdef _WIN32
     typedef SOCKET tcp_socket_t;
#else // _WIN32
     typedef int tcp_socket_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int SocketFormat_write(tcp_socket_t sd, const void* src, size_t size);

#ifdef __cplusplus
}
#endif

#endif // FLOTON_SOCKET_FORMAT_H