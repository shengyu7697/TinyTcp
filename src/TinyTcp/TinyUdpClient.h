#ifndef TINYUDPCLIENT_H
#define TINYUDPCLIENT_H

//#include <thread>
#include <functional>
#include <netinet/in.h>

class TinyUdpClient
{
public:
    TinyUdpClient();
    ~TinyUdpClient();

    int send(const char *data, int size);
    int start(const std::string &hostname, int port);
    void stop();
    inline bool isRunning() { return mRunning; }

private:
    void run();

    int mSocket;
    struct sockaddr_in mSrvAddr;
    std::string mHostname;
    int mPort;
    //std::thread mThread;
    bool mRunning;
};

#endif // TINYUDPCLIENT_H
