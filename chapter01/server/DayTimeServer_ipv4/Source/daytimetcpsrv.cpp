#include <iostream>
#include <time.h>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close				closesocket

#endif

#define MAX_SIZE			1023
#define MAX_CONNECT_NUM		256

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSAData SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler, ConnectHandler;
	struct sockaddr_in ServAddr;
	char Buffer[MAX_SIZE + 1];
	time_t Ticks;

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error" << std::endl;
	}

	memset(Buffer, 0, sizeof(Buffer));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServAddr.sin_port = htons(2017);

	if (bind(SocketHandler, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "bind error" << std::endl;
	}

	if (listen(SocketHandler, MAX_CONNECT_NUM) < 0)
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