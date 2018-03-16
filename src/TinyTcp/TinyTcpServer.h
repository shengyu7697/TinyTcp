#ifndef TINYTCPSERVER_H
#define TINYTCPSERVER_H

#include <thread>
#include <functional>
#include <vector>
#include <map>

typedef std::function<void(int session)> OnConnect;
typedef std::function<void(int session)> OnDisconnect;
typedef std::function<void(int session, const char *buf, int len)> OnRecv;

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
	int start(int port, int maxConn = 5);
	void closeConn(int session);

private:
	int bind(int port);
	int listen(int backlog);
	void run();
	void processConn(int socket, int session);

	int mServerSocket;
	int mPort;
	std::thread mThread;
	std::vector<std::thread> mConnThreadList; // server only
	std::map<int, int> mConnMap;
	int mSession;

	OnConnect onConnect;
	OnDisconnect onDisconnect;
	OnRecv onRecv;
};

#endif // TINYTCPSERVER_H
