#include "TinyTcpServer.h"
#include "SocketUtil.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

TinyTcpServer::TinyTcpServer() 
{

}

TinyTcpServer::~TinyTcpServer() 
{
	// TODO stop thread
}

void TinyTcpServer::setOnConnectCB(OnConnect onConnect)
{
	this->onConnect = onConnect;
}

void TinyTcpServer::setOnDisconnectCB(OnDisconnect onDisconnect)
{
	this->onDisconnect = onDisconnect;
}

void TinyTcpServer::setOnRecvCB(OnRecv onRecv)
{
	this->onRecv = onRecv;
}

int TinyTcpServer::send(const char *data, int size)
{
	this->send(0, data, size);
}

int TinyTcpServer::send(int session, const char *data, int size)
{
	// TODO send to socket? session?
	/*int len = ::send(mSocket, data, size, 0);
	if (len < 0) {
		printf("[TinyTcp] Send failed, error:\n"); // , getLastError()
	}
	return len;*/
}

int TinyTcpServer::sendAll(const char *data, int size)
{
}


int TinyTcpServer::start(int port, int maxConn)
{
	int sock_fd = createSocket();

	int on = 1;
	struct sockaddr_in my_name;
	int status;

	// set socket reuse
	// for "Address already in use" error message
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1) {
		perror("Setsockopt error");
		return -1;
	}

	// bind socket
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons(port);

	status = ::bind(sock_fd, (struct sockaddr *)&my_name, sizeof(my_name));
	if (status == -1) {
		perror("Binding error");
		return -1;
	}
	printf("server ip=%s:%d\n", inet_ntoa(my_name.sin_addr), port);


	//int status;
	status = ::listen(sock_fd, maxConn);
	if (status == -1) {
		perror("Listening error");
		return -1;
	}

	mServerSocket = sock_fd;

	mThread = std::thread(&TinyTcpServer::run, this);

	return 1;
}

int TinyTcpServer::run()
{
	int newSocket;

	struct sockaddr_in addr;
#ifdef _WIN32
	int addrlen = sizeof(addr);
#else
	socklen_t addrlen = sizeof(addr);
#endif

	while (1)
	{
		newSocket = accept(mServerSocket, (struct sockaddr *)&addr, &addrlen);
		printf("accecpt fd%d ip=%s:%d\n", newSocket,
				inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		if (newSocket == -1) {
			perror("Wrong connection");
			exit(1);
		}

		// notify onConnect callback
		if (onConnect)
			onConnect(mSession);

		// create a connection thread
		mConnThreadList.push_back(std::thread(&TinyTcpServer::processConn, this, newSocket, mSession));

		mSession++;
	}
}

void TinyTcpServer::processConn(int socket, int session)
{
	char buf[1024];

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int len = recv(socket, buf, sizeof(buf), 0); // TODO use read? recv?

		if (len == 0) { // connection closed
			// notify onRecv callback
			if (onDisconnect)
				onDisconnect(session);

			printf("[TinyTcp] close client socket=%d, session=%d\n", socket, session);
			closeSocket(socket); // close client socket
			break;
		} else if (len == -1) { // error
			printf("[TinyTcp] error %d\n", __LINE__);
			break;
		}

		// notify onRecv callback
		if (onRecv)
			onRecv(socket, session, buf, len);
	}
}
