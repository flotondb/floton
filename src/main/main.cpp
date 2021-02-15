#include <cstdio>
#include <signal.h>
#include "TcpServer.h"

static TcpServerThread* TCP_SERV_THREAD = nullptr;

void interSigHandler(int dummy)
{
	(void)dummy;
	TCP_SERV_THREAD->stop();
	std::puts("Shutting Down due to interrupt signal");
	exit(0);
}

int main(int argc, char const *argv[])
{
	TcpServerConfig config;
	signal(SIGINT, interSigHandler);
	std::puts("The Floton Database");
	std::puts("------------------------------");
	config.port = 23000;
	config.backLogCount = 500;
	config.threadPoolSize = 50;
	std::printf("Serving requests on port: %u\n", config.port);
	TCP_SERV_THREAD = new TcpServerThread(std::move(config));
	TCP_SERV_THREAD->go();
	TCP_SERV_THREAD->join();
	return 0;
}