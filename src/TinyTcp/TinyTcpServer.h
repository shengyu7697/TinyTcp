#ifndef TINYTCPSERVER_H
#define TINYTCPSERVER_H

#include <thread>
#include <functional>
#include <vector>
#include <map>

typedef std::function<void(int session)> OnConnect;
typedef std::function<void(int session)> OnDisconnect;
typedef std::function<void(int session, const char *buf, int len)> OnRecv;

class Connection {
public:
	std::thread thread;
	int socket;
};

class TinyTcpServer
{
public:
	TinyTcpServer();
	~TinyTcpServer();

	void setVerbose(int level);
	void setOnConnect(OnConnect onConnect);
	void setOnDisconnect(OnDisconnect onDisconnect);
	void setOnRecv(OnRecv onRecv);
	int send(const char *data, int size);
	int send(int session, const char *data, int size);
	int sendAll(const char *data, int size);
	int start(int port, int maxConn = 5);
	void closeConn(int session);
	void closeAllConn();
	bool isRunning();
	void stop();

private:
	int bind(int port);
	int listen(int backlog);
	void run();
	void processConn(int socket, int session);

	int mVerbose;
	int mServerSocket;
	int mPort;
	std::thread mThread;
	bool mRunning;
	std::map<int, Connection *> mConnections;
	int mSession;

	OnConnect onConnect;
	OnDisconnect onDisconnect;
	OnRecv onRecv;
};

#endif // TINYTCPSERVER_H
