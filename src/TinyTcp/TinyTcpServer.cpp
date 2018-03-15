#include "TinyTcpServer.h"
#include "SocketUtil.h"
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <string.h>

#include <iostream>

using namespace std;

TinyTcpServer::TinyTcpServer() 
{
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("[TinyTcp] WSAStartup failed, error: %d\n", WSAGetLastError());
	}
#endif
}

TinyTcpServer::~TinyTcpServer() 
{
	// TODO stop server thread
	//closeAllConn();
#ifdef _WIN32
	WSACleanup();
#endif
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
	return this->send(0, data, size);
}

int TinyTcpServer::send(int session, const char *data, int size)
{
	// find the socket of session and send
	auto c = mConnMap.find(session);
	if (c == end(mConnMap)) {
		printf("[TinyTcp] session %d is not exist!\n", session);
		return -1;
	}

	int len = ::send(c->second, data, size, 0);
	if (len < 0) {
		printf("[TinyTcp] Send failed, error:\n"); // FIXME getLastError()
	}
	return len;
}

int TinyTcpServer::sendAll(const char *data, int size)
{
	int len;
	map<int, int>::iterator c;
	for(c = mConnMap.begin(); c != mConnMap.end(); c++) {
		printf("send to %d %d\n", c->first, c->second);
		len = ::send(c->second, data, size, 0);
		if (len < 0) {
			printf("[TinyTcp] Send failed, error:\n"); // FIXME getLastError()
		}
	}
	return len;
}

int TinyTcpServer::start(int port, int maxConn)
{
	int s = createSocket();
	if (s < 0)
		return -1;

	// set socket reuse
	int optval = 1;
#ifdef _WIN32
	int optlen = sizeof(optval);
#else
	socklen_t optlen = sizeof(optval);
#endif
	int status = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, optlen);
	if (status == -1) {
		perror("Setsockopt error");
		return -1;
	}

	// bind socket
	struct sockaddr_in my_name;
	my_name.sin_family = AF_INET;
	my_name.sin_addr.s_addr = INADDR_ANY;
	my_name.sin_port = htons(port);

	status = ::bind(s, (struct sockaddr *)&my_name, sizeof(my_name));
	if (status == -1) {
		perror("Binding error");
		return -1;
	}
	printf("server ip=%s:%d\n", inet_ntoa(my_name.sin_addr), port);

	status = ::listen(s, maxConn);
	if (status == -1) {
		perror("Listening error");
		return -1;
	}

	mServerSocket = s;

	mThread = std::thread(&TinyTcpServer::run, this);

	return 1;
}

void TinyTcpServer::run()
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
		if (newSocket == -1) {
			printf("[TinyTcp] Accept failed, error: \n"); // FIXME getLastError()
			break;
		}

		printf("accecpt fd%d ip=%s:%d\n", newSocket,
				inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		// notify onConnect callback
		if (onConnect)
			onConnect(mSession);

		// create a connection thread
		mConnThreadList.push_back(std::thread(&TinyTcpServer::processConn, this, newSocket, mSession));
		mConnMap.insert(pair<int, int>(mSession, newSocket));

		mSession++;
	}

	closeSocket(mServerSocket);
}

void TinyTcpServer::processConn(int socket, int session)
{
	char buf[1024];

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int len = recv(socket, buf, sizeof(buf), 0); // TODO use read? recv?

		if (len == 0) { // connection closed
			printf("[TinyTcp] close client socket=%d, session=%d\n", socket, session);
			break;
		} else if (len == -1) { // error
			printf("[TinyTcp] error %d\n", __LINE__);
			break;
		}

		// notify onRecv callback
		if (onRecv)
			onRecv(session, buf, len);
	}

	closeConn(session);
}

void TinyTcpServer::closeConn(int session)
{
	auto c = mConnMap.find(session);
	if (c == end(mConnMap))
		return;

	// notify onDisconnect callback
	if (onDisconnect)
		onDisconnect(session);

	closeSocket(c->second); // close client socket

	mConnMap.erase(c);

	// TODO stop conn thread
}
