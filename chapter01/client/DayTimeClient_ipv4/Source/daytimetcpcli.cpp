#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close		closesocket

#endif

#define MAX_SIZE				1023

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: DayTimeClient_ipv4.exe <ip-address>" << std::endl;
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler, n;
	char Buffer[MAX_SIZE + 1];
	struct sockaddr_in ServAddr;

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error" << std::endl;
	}

	memset(Buffer, 0, sizeof(Buffer));
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(2017);

	if (inet_pton(AF_INET, argv[1], &ServAddr.sin_addr) <= 0)
	{
		std::cerr << "inet_pton error for " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	if (connect(SocketHandler, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "connect error" << std::endl;
	}

	while ((n = recv(SocketHandler, Buffer, MAX_SIZE, 0)) > 0)
	{
		Buffer[n] = '\0';
		if (fputs(Buffer, stdout) == EOF)
		{
			std::cerr << "fputs error" << std::endl;
		}
	}

	if (n < 0)
	{
		std::cerr << "read error" << std::endl;
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