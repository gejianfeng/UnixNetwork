#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close					closesocket

#endif

#define MAX_SIZE				255

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: TcpClient01_ipv4.exe <ip-address>" << std::endl;
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler;
	struct sockaddr_in ServAddr;
	char RecvMsg[MAX_SIZE + 1];
	char SendMsg[MAX_SIZE + 1];
	int n;

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error." << std::endl;
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	memset(RecvMsg, 0, sizeof(RecvMsg));
	memset(SendMsg, 0, sizeof(SendMsg));

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

	while (std::cin.getline(SendMsg, MAX_SIZE))
	{
		if (strcmp(SendMsg, "exit") == 0)
		{
			if (SocketHandler >= 0)
			{
				close(SocketHandler);
				break;
			}
		}

		send(SocketHandler, SendMsg, strlen(SendMsg), 0);

		if ((n = recv(SocketHandler, RecvMsg, sizeof(RecvMsg), 0)) > 0)
		{
			RecvMsg[n] = '\0';

			if (fputs(RecvMsg, stdout) == EOF)
			{
				std::cerr << "fputs error." << std::endl;
			}
			else
			{
				std::cout << std::endl;
			}
		}

		if (n < 0)
		{
			std::cerr << "recv error." << std::endl;
		}
	}

#ifdef _WIN32
	WSACleanup();
#endif

	return EXIT_SUCCESS;
}