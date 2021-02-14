#include "SocketFormat.h"

extern int SocketFormat_write(tcp_socket_t sd, const void* src, size_t size)
{
	return send(sd, src, size, 0) != -1;
}

extern int SocketFormat_read(tcp_socket_t sd, void* dst, size_t size)
{
	return recv(sd, dst, size, 0) > 0;
}

extern int SocketFormat_close(tcp_socket_t sd)
{
#ifdef _WIN32
	return closesocket(sd) == 0;
#else
	return close(sd) == 0;
#endif
}