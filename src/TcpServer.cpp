#include "TcpServer.h"

TcpConnection::TcpConnection(int sd): _sd(sd) {}

TcpConnection::~TcpConnection()
{
	close(_sd);
}