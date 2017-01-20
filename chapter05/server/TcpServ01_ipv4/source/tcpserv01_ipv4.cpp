#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close					closesocket

#endif

#define MAX_CONNECT_NUM			16
#define MAX_BUFFER_SIZE			255

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler, ClientHandler;
	struct sockaddr_in ServAddr;

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error." << std::endl;
		return EXIT_FAILURE;
	}

	memset(&ServAddr, 0, sizeof(ServAddr));

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(60000);
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(SocketHandler, (sockaddr*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "bind error." << std::endl;
		return EXIT_FAILURE;
	}

	if (listen(SocketHandler, MAX_CONNECT_NUM) < 0)
	{
		std::cerr << "listen error." << std::endl;
		return EXIT_FAILURE;
	}

	for (;;)
	{
		ClientHandler = accept(SocketHandler, (sockaddr*)nullptr, nullptr);

		struct sockaddr_storage Addr;
		int AddrLen = sizeof(Addr);
		if (getpeername(ClientHandler, (sockaddr*)&Addr, &AddrLen) < 0)
		{
			std::cerr << "getpeername error." << std::endl;
			close(ClientHandler);
			continue;
		}

		char Buffer[MAX_CONNECT_NUM + 1];

		char IpAddress[MAX_BUFFER_SIZE + 1];
		char Port[MAX_BUFFER_SIZE + 1];

		int n = 0;

		memset(IpAddress, 0, sizeof(IpAddress));
		memset(Port, 0, sizeof(Port));
		memset(Buffer, 0, sizeof(Buffer));

		switch (Addr.ss_family)
		{
		case AF_INET:
		{
			struct sockaddr_in ClientAddr = *((sockaddr_in*)&Addr);

			if (inet_ntop(Addr.ss_family, &ClientAddr.sin_addr, IpAddress, sizeof(IpAddress)) <= 0)
			{
				std::cerr << "inet_ntop error." << std::endl;
				close(ClientHandler);
				continue;
			}

			snprintf(Port, sizeof(Port), "%d", ntohs(ClientAddr.sin_port));

			break;
		}
		case AF_INET6:
		{
			struct sockaddr_in6 ClientAddr = *((sockaddr_in6*)&Addr);

			if (inet_ntop(Addr.ss_family, &ClientAddr.sin6_addr, IpAddress, sizeof(IpAddress)) <= 0)
			{
				std::cerr << "inet_ntop error." << std::endl;
				close(ClientHandler);
				continue;
			}

			snprintf(Port, sizeof(Port), "%d", ntohs(ClientAddr.sin6_port));

			break;
		}
		default:
		{
			std::cerr << "ipv4 or ipv6 connection." << std::endl;
			close(ClientHandler);
			continue;

			break;
		}
		}

		while (true)
		{
			n = recv(ClientHandler, Buffer, sizeof(Buffer), 0);

			if (n > 0)
			{
				Buffer[n] = '\0';

				if (fputs(Buffer, stdout) == EOF)
				{
					std::cerr << "fputs error." << std::endl;
					close(ClientHandler);
					continue;
				}
				else
				{
					std::cout << std::endl;
					send(ClientHandler, Buffer, strlen(Buffer), 0);
				}
			}
			else if (n < 0)
			{
				std::cerr << "recv error." << std::endl;
				close(ClientHandler);
				break;
			}
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