#ifndef TINYUDPSERVER_H
#define TINYUDPSERVER_H

#include <thread>
#include <functional>

class TinyUdpServer
{
    typedef std::function<void(const char *buf, int len)> OnRecv;
public:
    TinyUdpServer();
    ~TinyUdpServer();

    void setOnRecv(OnRecv onRecv);
    int start(int port);
    void stop();
    inline bool isRunning() { return mRunning; }

private:
    void run();

    int mSocket;
    int mPort;
    std::thread mThread;
    bool mRunning;

    OnRecv onRecv;
};

#endif // TINYUDPSERVER_H
