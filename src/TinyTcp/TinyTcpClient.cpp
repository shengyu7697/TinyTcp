#include "TinyTcpClient.h"
#include "SocketUtil.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

TinyTcpClient::TinyTcpClient() 
{
}

TinyTcpClient::~TinyTcpClient() 
{
	// TODO stop thread
}

void TinyTcpClient::setOnConnectCB(OnConnect onConnect)
{
	this->onConnect = onConnect;
}

void TinyTcpClient::setOnDisconnectCB(OnDisconnect onDisconnect)
{
	this->onDisconnect = onDisconnect;
}

void TinyTcpClient::setOnRecvCB(OnRecv onRecv)
{
	this->onRecv = onRecv;
}

int TinyTcpClient::send(const char *data, int size)
{
	int len = ::send(mSocket, data, size, 0);
	if (len < 0) {
		printf("[TinyTcp] Send failed, error:\n"); // , getLastError()
	}
	return len;
}

int TinyTcpClient::start(const std::string &hostname, int port)
{
	int s;
	if ((s = createSocket()) < 0)
		return -1;

	mSocket = s;
	mHostname = hostname;
	mPort = port;

	mThread = std::thread(&TinyTcpClient::run, this);
}

int TinyTcpClient::run()
{
	// server address
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(mPort);
	inet_pton(AF_INET, mHostname.c_str(), &(addr.sin_addr)); // TODO use inet_aton ?
	//inet_aton(ip_address, &addr.sin_addr);

	while (!mConnected) // retry forever
	{
		// connect to the server
		int ret = ::connect(mSocket, (struct sockaddr *)&addr, sizeof(addr));
		if (ret == 0) {
			mConnected = true;
		} else if (ret == -1) { // -1 shall be returned and errno set to indicate the error.
			perror("Connection error");
			return -1;
		}
	}

	// notify onConnect callback
	if (onConnect)
		onConnect(mSession);

	processConn(mSocket, mSession);
}

void TinyTcpClient::processConn(int socket, int session)
{
	char buf[1024];

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		int len = recv(socket, buf, sizeof(buf), 0);

		if (len == 0) { // connection closed
			// notify onRecv callback
			if (onDisconnect)
				onDisconnect(session);

			printf("[TinyTcp] recv server socket closed\n");
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
