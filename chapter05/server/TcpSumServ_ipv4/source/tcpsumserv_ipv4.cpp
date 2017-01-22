#include <iostream>
#include "sum.h"

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
	struct args Param;
	struct result Ret;
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

		while (true)
		{
			n = recv(ClientHandler, Buffer, sizeof(struct args), 0);

			if (n > 0)
			{
				memcpy(&Param, Buffer, sizeof(struct args));
				Ret.sum = Param.arg1 + Param.arg2;
				std::cout << Param.arg1 << " + " << Param.arg2 << " = " << Ret.sum << std::endl;
				send(ClientHandler, (char*)&Ret, sizeof(Ret), 0);
			}
			else if (n < 0)
			{
				std::cerr << "recv error." << std::endl;
				continue;
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