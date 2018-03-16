#include "SocketUtil.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

int createSocket()
{
	int s;
	// create a socket
	s = socket(AF_INET, SOCK_STREAM, 0);
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
	//shutdown(socket, SHUT_RDWR); // FIXME
	close(socket);
#endif
	//printf("[TinyTcp] Socket closed\n");
}

int getLastError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}