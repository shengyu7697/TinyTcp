#ifndef TINYTCPCLIENT_H
#define TINYTCPCLIENT_H

#include <thread>
#include <functional>

typedef std::function<void(int session)> OnConnect;
typedef std::function<void(int session)> OnDisconnect;
typedef std::function<void(int session, const char *buf, int len)> OnRecv;

class TinyTcpClient
{
public:
	TinyTcpClient();
	~TinyTcpClient();

	void setVerbose(int level);
	void setOnConnect(OnConnect onConnect);
	void setOnDisconnect(OnDisconnect onDisconnect);
	void setOnRecv(OnRecv onRecv);
	int send(const char *data, int size);
	int start(const std::string &hostname, int port);
	bool isRunning();
	void stop();

private:
	void run();
	void processConn(int socket, int session);

	int mVerbose;
	int mSocket;
	std::string mHostname;
	int mPort;
	std::thread mThread;
	bool mRunning;
	int mSession;

	OnConnect onConnect;
	OnDisconnect onDisconnect;
	OnRecv onRecv;
};

#endif // TINYTCPCLIENT_H
