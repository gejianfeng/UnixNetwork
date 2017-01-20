#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close					closesocket

#endif

#define MAX_SIZE				1023
#define MAX_CONNECT_NUM			16

void ProcessTask(int SocketHandler);

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
	}

	memset(&ServAddr, 0, sizeof(ServAddr));

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(60000);
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

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
		ClientHandler = accept(SocketHandler, (sockaddr*)nullptr, nullptr);
		ProcessTask(ClientHandler);
		//close(ClientHandler);
	}

	if (ClientHandler >= 0)
	{
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

void ProcessTask(int SocketHandler)
{
	char IpAddress[MAX_SIZE + 1];
	char Port[MAX_SIZE + 1];

	char Buffer[MAX_SIZE + 1];

	int n;

	memset(IpAddress, 0, sizeof(IpAddress));
	memset(Port, 0, sizeof(Port));
	memset(Buffer, 0, sizeof(Buffer));

	struct sockaddr_storage TempAddr;
	int TempAddrLen = sizeof(TempAddr);

	if (getpeername(SocketHandler, (sockaddr*)&TempAddr, &TempAddrLen) < 0)
	{
		std::cerr << "getpeername error." << std::endl;
		return;
	}

	switch (TempAddr.ss_family)
	{
	case AF_INET:
	{
		struct sockaddr_in ClientAddr = *((sockaddr_in*)&TempAddr);

		if (inet_ntop(TempAddr.ss_family, &ClientAddr.sin_addr, IpAddress, sizeof(IpAddress)) <= 0)
		{
			std::cerr << "inet_ntop error." << std::endl;
			return;
		}

		snprintf(Port, sizeof(Port), "%d", ntohs(ClientAddr.sin_port));
	}
		break;
	case AF_INET6:
	{
		struct sockaddr_in6 ClientAddr = *((sockaddr_in6*)&TempAddr);

		if (inet_ntop(TempAddr.ss_family, &ClientAddr.sin6_addr, IpAddress, sizeof(IpAddress)) <= 0)
		{
			std::cerr << "inet_ntop error." << std::endl;
			return;
		}

		snprintf(Port, sizeof(Port), "%d", ntohs(ClientAddr.sin6_port));
	}
		break;
	default:
		return;
		break;
	}

	while ((n = recv(SocketHandler, Buffer, sizeof(Buffer), 0)) > 0)
	{
		Buffer[n] = '\0';
		std::cout << IpAddress << ":" << Port << " - Recv: " << Buffer << std::endl;

		send(SocketHandler, Buffer, strlen(Buffer), 0);
		std::cout << IpAddress << ":" << Port << " - Send: " << Buffer << std::endl;

		memset(Buffer, 0, sizeof(Buffer));
	}
}