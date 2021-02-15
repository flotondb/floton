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

#define FLOTON_SOCKET_FORMAT_ECHO_TEXT_SIZE 256

extern int SocketFormat_echo_text(tcp_socket_t sd)
{
	char msg[FLOTON_SOCKET_FORMAT_ECHO_TEXT_SIZE] = {0};
	return SocketFormat_read(sd, msg, FLOTON_SOCKET_FORMAT_ECHO_TEXT_SIZE) &&
	       SocketFormat_write(sd, msg, FLOTON_SOCKET_FORMAT_ECHO_TEXT_SIZE);
}

#undef FLOTON_SOCKET_FORMAT_ECHO_TEXT_SIZE