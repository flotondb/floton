#include "SocketFormat.h"

extern int SocketFormat_write(tcp_socket_t sd, const void* src, size_t size)
{
	return send(_sd, src, size, 0) != -1;
}