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
#ifdef _WIN32
	WSADATA SocketData;
	WSAStartup(MAKEWORD(2, 2), &SocketData);
#endif

	const char* StrIpv4Addr = "10.1.230.133";
	const char* StrIpv6Addr = "fe80::2487:e76c:69bd:10fa";

	struct sockaddr_in Ipv4Addr;
	struct sockaddr_in6 Ipv6Addr;

	char Ipv4AddrRet[MAX_SIZE + 1];
	char Ipv6AddrRet[MAX_SIZE + 1];

	// ipv4
	memset(&Ipv4Addr, 0, sizeof(Ipv4Addr));
	memset(Ipv4AddrRet, 0, sizeof(Ipv4AddrRet));

	if (inet_pton(AF_INET, StrIpv4Addr, &Ipv4Addr.sin_addr) <= 0)
	{
		std::cerr << "inet_pton error for " << StrIpv4Addr << "." << std::endl;
	}
	else
	{
		std::cout << "inet_pton success for " << StrIpv4Addr << "." << std::endl;
	}

	if (inet_ntop(AF_INET, &Ipv4Addr.sin_addr, Ipv4AddrRet, sizeof(Ipv4AddrRet)) < 0)
	{
		std::cerr << "inet_ntop error for " << StrIpv4Addr << "." << std::endl;
	}
	else
	{
		std::cout << "inet_ntop success for " << Ipv4AddrRet << "." << std::endl;
	}

	// ipv6
	memset(&Ipv6Addr, 0, sizeof(Ipv6Addr));
	memset(&Ipv6AddrRet, 0, sizeof(Ipv6AddrRet));

	if (inet_pton(AF_INET6, StrIpv6Addr, &Ipv6Addr.sin6_addr) <= 0)
	{
		std::cerr << "inet_pton error for " << StrIpv6Addr << "." << std::endl;
	}
	else
	{
		std::cout << "inet_pton success for " << StrIpv6Addr << "." << std::endl;
	}

	if (inet_ntop(AF_INET6, &Ipv6Addr.sin6_addr, Ipv6AddrRet, sizeof(Ipv6AddrRet)) < 0)
	{
		std::cerr << "inet_ntop error for " << StrIpv6Addr << "." << std::endl;
	}
	else
	{
		std::cout << "inet_ntop success for " << Ipv6AddrRet << "." << std::endl;
	}

#ifdef _WIN32
	WSACleanup();
#endif

	return EXIT_SUCCESS;
}