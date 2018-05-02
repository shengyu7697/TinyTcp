#ifndef TINYTCPCLIENT_H
#define TINYTCPCLIENT_H

#include <thread>
#include <functional>

class TinyTcpClient
{
    typedef std::function<void(int session)> OnConnect;
    typedef std::function<void(int session)> OnDisconnect;
    typedef std::function<void(int session, const char *buf, int len)> OnRecv;
public:
    TinyTcpClient();
    ~TinyTcpClient();

    void setVerbose(int level);
    void setOnConnect(OnConnect onConnect);
    void setOnDisconnect(OnDisconnect onDisconnect);
    void setOnRecv(OnRecv onRecv);
    int send(const char *data, int size);
    int start(const std::string &hostname, int port);
    void stop();
    inline bool isRunning() { return mRunning; }
    inline bool isConnected() { return mConnected; }

private:
    void run();
    void processConn(int socket, int session);

    int mVerbose;
    int mSocket;
    std::string mHostname;
    int mPort;
    std::thread mThread;
    bool mRunning;
    bool mConnected;
    int mSession;

    OnConnect onConnect;
    OnDisconnect onDisconnect;
    OnRecv onRecv;
};

#endif // TINYTCPCLIENT_H
