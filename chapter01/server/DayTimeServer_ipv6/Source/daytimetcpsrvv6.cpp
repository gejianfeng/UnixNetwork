#include <iostream>
#include <time.h>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close closesocket

#endif

#define MAX_SIZE 1023
#define MAX_CONNECT_NUM 256

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler, ConnectHandler;
	struct sockaddr_in6 ServAddr;
	char Buffer[MAX_SIZE + 1];
	time_t Ticks;

	SocketHandler = socket(AF_INET6, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error" << std::endl;
	}

	memset(Buffer, 0, sizeof(Buffer));
	memset(&ServAddr, 0, sizeof(ServAddr));

	ServAddr.sin6_family = AF_INET6;
	ServAddr.sin6_port = htons(2017);
	ServAddr.sin6_addr = in6addr_any;

	if (bind(SocketHandler, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "bind error" << std::endl;
	}

	if (listen(SocketHandler, MAX_CONNECT_NUM))
	{
		std::cerr << "listen error" << std::endl;
	}

	for (;;)
	{
		ConnectHandler = accept(SocketHandler, (SOCKADDR*)nullptr, nullptr);
		Ticks = time(nullptr);
		snprintf(Buffer, sizeof(Buffer), "%.24s\r\n", ctime(&Ticks));
		send(ConnectHandler, Buffer, strlen(Buffer), 0);
		close(ConnectHandler);
	}

	if (SocketHandler >= 0)
	{
		close(SocketHandler);
	}

#ifdef _WIN32
	WSACleanup();
#endif

	return EXIT_SUCCESS;
}