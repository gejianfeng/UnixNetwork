#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close				closesocket

#endif

#define MAX_SIZE			255

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: GetSockNameClient_ipv4.exe <ip-address>." << std::endl;
		return EXIT_FAILURE;
	}

#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler;
	struct sockaddr_in ServAddr;
	char Buffer[MAX_SIZE + 1];

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error" << std::endl;
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
	}

	struct sockaddr_storage LocalAddr;
	int AddrLen = sizeof(LocalAddr);

	if (getsockname(SocketHandler, (sockaddr*)&LocalAddr, &AddrLen) < 0)
	{
		std::cerr << "getsockname error." << std::endl;
	}
	else
	{
		struct sockaddr_in* TempIpv4AddrPtr = nullptr;

		switch (LocalAddr.ss_family)
		{
		case AF_INET:
			std::cout << "AF_INET connection." << std::endl;

			TempIpv4AddrPtr = (sockaddr_in*)&LocalAddr;

			if (TempIpv4AddrPtr)
			{
				if (inet_ntop(LocalAddr.ss_family, &((*TempIpv4AddrPtr).sin_addr), Buffer, sizeof(Buffer)))
				{
					std::cout << "address: " << Buffer << " , port: " << ntohs((*TempIpv4AddrPtr).sin_port) << std::endl;
				}
				else
				{
					std::cerr << "inet_ntop error." << std::endl;
				}
			}
			else
			{
				std::cerr << "convert from sockaddr_storage => sockaddr_in error." << std::endl;
			}

			break;
		case AF_INET6:
			std::cout << "AF_INET6 connection." << std::endl;
			break;
		default:
			std::cout << "Other connection." << std::endl;
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