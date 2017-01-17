#include <iostream>

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define close closesocket

#endif

#define MAX_SIZE 1023

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	int SocketHandler;
	SocketHandler = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketHandler < 0)
	{
		std::cerr << "socket error" << std::endl;
		return EXIT_FAILURE;
	}

	// ipv4
	struct sockaddr_in ServAddrIpv4;
	std::string Ipv4Address = "192.168.1.1";

	memset(&ServAddrIpv4, 0, sizeof(ServAddrIpv4));

	if (inet_pton(AF_INET, Ipv4Address.c_str(), &ServAddrIpv4.sin_addr) < 0)
	{
		std::cerr << "inet_pton (ipv4) error" << std::endl;
		return EXIT_FAILURE;
	}

	char Ipv4AddressRet[MAX_SIZE + 1];
	memset(Ipv4AddressRet, 0, sizeof(Ipv4AddressRet));

	if (inet_ntop(AF_INET, &ServAddrIpv4.sin_addr, Ipv4AddressRet, sizeof(Ipv4AddressRet)) == nullptr)
	{
		std::cerr << "inet_ntop (ipv4) error" << std::endl;
		return EXIT_FAILURE;
	}

	// ipv6
	struct sockaddr_in6 ServAddrIpv6;
	std::string Ipv6Address = "fe80::2487:e76c:69bd:10fa";

	memset(&ServAddrIpv6, 0, sizeof(ServAddrIpv6));

	if (inet_pton(AF_INET6, Ipv6Address.c_str(), &ServAddrIpv6.sin6_addr) < 0)
	{
		std::cerr << "inet_pton (ipv6) error" << std::endl;
		return EXIT_FAILURE;
	}

	char Ipv6AddressRet[MAX_SIZE + 1];
	memset(Ipv6AddressRet, 0, sizeof(Ipv6AddressRet));

	if (inet_ntop(AF_INET6, &ServAddrIpv6.sin6_addr, Ipv6AddressRet, sizeof(Ipv6AddressRet)) == nullptr)
	{
		std::cerr << "inet_ntop (ipv6) error" << std::endl;
		return EXIT_FAILURE;
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