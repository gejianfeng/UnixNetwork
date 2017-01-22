#include <iostream>
#include "sum.h"

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close					closesocket

#endif

#define MAX_BUFFER_SIZE			255

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: TcpSumClient_ipv4.exe <ip-address>" << std::endl;
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler;
	struct sockaddr_in ServAddr;
	char Buffer[MAX_BUFFER_SIZE + 1];
	int n = 0;

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error." << std::endl;
		return EXIT_FAILURE;
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	memset(Buffer, 0, sizeof(Buffer));

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(60000);

	if (inet_pton(AF_INET, argv[1], &ServAddr.sin_addr) <= 0)
	{
		std::cerr << "inet_pton error for " << argv[1] << "." << std::endl;
		return EXIT_FAILURE;
	}

	if (connect(SocketHandler, (sockaddr*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "connect error." << std::endl;
		return EXIT_FAILURE;
	}

	while (std::cin.getline(Buffer, sizeof(Buffer)))
	{
		struct args Param;
		if (sscanf_s(Buffer, "%ld%ld", &Param.arg1, &Param.arg2) != 2)
		{
			std::cout << "invalid input: " << Buffer << std::endl;
			continue;
		}

		memset(Buffer, 0, sizeof(Buffer));
		memcpy(Buffer, &Param, sizeof(struct args));

		send(SocketHandler, Buffer, sizeof(struct args), 0);

		struct result Ret;
		if ((n = recv(SocketHandler, (char*)&Ret, sizeof(struct result), 0)) > 0)
		{
			std::cout << Ret.sum << std::endl;
		}

		if (n < 0)
		{
			std::cerr << "recv error." << std::endl;
			break;
		}
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