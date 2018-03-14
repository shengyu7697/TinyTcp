#ifndef TINYTCPCLIENT_H
#define TINYTCPCLIENT_H

#include <thread>
#include <functional>

typedef std::function<void(int session)> OnConnect;
typedef std::function<void(int session)> OnDisconnect;
typedef std::function<void(int socket, int session, const char *buf, int len)> OnRecv;

class TinyTcpClient
{
public:
	TinyTcpClient();
	~TinyTcpClient();

	void setOnConnectCB(OnConnect onConnect);
	void setOnDisconnectCB(OnDisconnect onDisconnect);
	void setOnRecvCB(OnRecv onRecv);
	int send(const char *data, int size);
	int start(const std::string &hostname, int port);

private:
	int run();
	void processConn(int socket, int session);

	int mSocket;
	std::string mHostname;
	int mPort;
	std::thread mThread;
	bool mConnected; // client only
	int mSession = 0;

	OnConnect onConnect = nullptr;
	OnDisconnect onDisconnect = nullptr;
	OnRecv onRecv = nullptr;
};

#endif // TINYTCPCLIENT_H
