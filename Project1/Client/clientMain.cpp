#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5150"

int main(int argc, char** argv)
{
	WSADATA wsaData;
	int _result;

	_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (_result != 0)
	{
		printf("WSAStartup failed with error: %d\n", _result);
		return 1;
	}
	else
	{
		printf("WSAStartup() was successful!\n");
	}

	SOCKET connectSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	_result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (_result != 0)
	{
		printf("getaddrinfo() failed with error: %d\n", _result);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("getaddrinfo() successful!\n");
	}
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET)
		{
			printf("socket() failed with error code %d\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to the server
		_result = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (_result == SOCKET_ERROR)
		{
			printf("connect() failed with error code %d\n", WSAGetLastError());
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to the server!\n");
		WSACleanup();
		return 1;
	}
	printf("Successfully connected to the server on socket %d!\n", connectSocket);

	const char* buffer = "Hello server!";

	printf("Sending a packet to the server...\n");
	_result = send(connectSocket, buffer, (int)strlen(buffer), 0);
	if (_result == SOCKET_ERROR)
	{
		printf("send() failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes sent: %d\n", _result);

	// Receive a message from the server before quitting
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	printf("Waiting to receive data from the server...\n");
	_result = recv(connectSocket, recvbuf, recvbuflen, 0);
	if (_result > 0)
	{
		printf("Bytes received: %d\n", _result);
	}
	else if (_result == 0)
	{
		printf("Connection closed\n");
	}
	else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(connectSocket);
	WSACleanup();

	return 0;
}