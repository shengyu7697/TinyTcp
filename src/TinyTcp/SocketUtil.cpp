#include "SocketUtil.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#endif

int createSocket()
{
	int s;
	// create a socket
	s = (int)socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		//perror("Socket creation error");
		return -1;
	}
	//printf("[TinyTcp] Socket created\n");

	return s;
}

void closeSocket(int socket)
{
#ifdef _WIN32
	closesocket(socket);
#else
	shutdown(socket, SHUT_RDWR);
	close(socket);
#endif
	//printf("[TinyTcp] Socket closed\n");
}

int acceptSocket(int socket)
{
	struct sockaddr_in addr;
#ifdef _WIN32
	int addrlen = sizeof(addr);
#else
	socklen_t addrlen = sizeof(addr);
#endif

	return (int)accept(socket, (struct sockaddr *)&addr, &addrlen);
}

int getLastError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}
