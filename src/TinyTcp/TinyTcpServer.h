#ifndef TINYTCPSERVER_H
#define TINYTCPSERVER_H

#include <thread>
#include <functional>
#include <vector>

typedef std::function<void(int session)> OnConnect;
typedef std::function<void(int session)> OnDisconnect;
typedef std::function<void(int socket, int session, const char *buf, int len)> OnRecv;

class TinyTcpServer
{
public:
	TinyTcpServer();
	~TinyTcpServer();

	void setOnConnectCB(OnConnect onConnect);
	void setOnDisconnectCB(OnDisconnect onDisconnect);
	void setOnRecvCB(OnRecv onRecv);
	int send(const char *data, int size);
	int send(int session, const char *data, int size);
	int sendAll(const char *data, int size);
	int start(int port, int maxConn);

private:
	int bind(int port);
	int listen(int backlog);
	int run();
	void processConn(int socket, int session);

	int mServerSocket;
	int mPort;
	std::thread mThread;
	std::vector<std::thread> mConnThreadList; // server only
	int mSession = 0;

	OnConnect onConnect = nullptr;
	OnDisconnect onDisconnect = nullptr;
	OnRecv onRecv = nullptr;
};

#endif // TINYTCPSERVER_H
