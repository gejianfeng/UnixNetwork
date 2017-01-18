#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close				closesocket

#endif

#define MAX_SIZE			255
#define MAX_CONNECT_NUM		16

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler, ClientHandler;
	struct sockaddr_in ServAddr, ClientAddr;
	char Buffer[MAX_SIZE + 1];

	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error." << std::endl;
	}

	memset(&ServAddr, 0, sizeof(ServAddr));
	
	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(60000);
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int AddrLen = sizeof(ClientAddr);

	if (bind(SocketHandler, (sockaddr*)&ServAddr, sizeof(ServAddr)) < 0)
	{
		std::cerr << "bind error." << std::endl;
	}

	if (listen(SocketHandler, MAX_CONNECT_NUM) < 0)
	{
		std::cerr << "listen error." << std::endl;
	}

	for (;;)
	{
		ClientHandler = accept(SocketHandler, (sockaddr*)&ClientAddr, &AddrLen);
		
		memset(Buffer, 0, sizeof(Buffer));
		if (inet_ntop(AF_INET, &ClientAddr.sin_addr, Buffer, sizeof(Buffer)) == nullptr)
		{
			std::cerr << "inet_ntop error for client address convert." << std::endl;
		}
		else
		{
			std::cout << "client address: " << Buffer << " , port: " << ntohs(ClientAddr.sin_port) << std::endl;
		}

		close(ClientHandler);
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