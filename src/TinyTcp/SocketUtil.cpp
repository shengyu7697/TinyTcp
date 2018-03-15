#include "SocketUtil.h"

#include <sys/socket.h>
#include <unistd.h>

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
