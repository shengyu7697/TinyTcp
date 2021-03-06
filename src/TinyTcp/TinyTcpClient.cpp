#include "TinyTcpClient.h"
#include "SocketUtil.h"
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <string.h>

TinyTcpClient::TinyTcpClient() :
    mVerbose(0),
    mSocket(-1),
    mRunning(true),
    mConnected(false),
    mSession(0),
    onConnect(nullptr),
    onDisconnect(nullptr),
    onRecv(nullptr)
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[TinyTcp] WSAStartup failed, error: %d\n", WSAGetLastError());
    }
#endif
}

TinyTcpClient::~TinyTcpClient() 
{
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void TinyTcpClient::setVerbose(int level)
{
    mVerbose = level;
}

void TinyTcpClient::setOnConnect(OnConnect onConnect)
{
    this->onConnect = onConnect;
}

void TinyTcpClient::setOnDisconnect(OnDisconnect onDisconnect)
{
    this->onDisconnect = onDisconnect;
}

void TinyTcpClient::setOnRecv(OnRecv onRecv)
{
    this->onRecv = onRecv;
}

int TinyTcpClient::send(const char *data, int size)
{
    if (!mConnected)
        return -1;

    int len = ::send(mSocket, data, size, 0);
    if (len < 0) {
        printf("[TinyTcp] Send failed, error:\n"); // FIXME getLastError()
    }
    return len;
}

int TinyTcpClient::start(const std::string &hostname, int port)
{
    int s = createTcpSocket();
    if (s < 0)
        return -1;

    mSocket = s;
    mHostname = hostname;
    mPort = port;

    mThread = std::thread(&TinyTcpClient::run, this);
    mConnected = false;
    return 1;
}

void TinyTcpClient::run()
{
    // server address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(mPort);
    inet_pton(AF_INET, mHostname.c_str(), &(addr.sin_addr));

    while (!mConnected && mSocket != -1) // retry forever
    {
        // connect to the server
        int ret = ::connect(mSocket, (struct sockaddr *)&addr, sizeof(addr));
        if (ret == 0) {
            mConnected = true;
        } else if (ret == -1) { // -1 shall be returned and errno set to indicate the error.
            perror("Connect error");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 1 second
        }
    }

    // notify onConnect callback
    if (onConnect && mConnected)
        onConnect(mSession);

    processConn(mSocket, mSession);
}

void TinyTcpClient::processConn(int socket, int session)
{
    char buf[1024];

    while (mRunning)
    {
        memset(buf, 0, sizeof(buf));
        int len = recv(socket, buf, sizeof(buf), 0);

        if (len == 0) { // connection closed
            printf("[TinyTcp] recv server socket closed\n");
            break;
        } else if (len == -1) { // error
            printf("[TinyTcp] error recv %d\n", __LINE__);
            break;
        }

        // notify onRecv callback
        if (onRecv)
            onRecv(session, buf, len);
    }

    // notify onDisconnect callback
    if (onDisconnect)
        onDisconnect(session);

    closeSocket(socket); // close client socket

    mRunning = false;
    mConnected = false;
}

void TinyTcpClient::stop()
{
    mConnected = false;

    if (mThread.joinable()) {
        closeSocket(mSocket); // close client self socket
        mSocket = -1;
        mThread.join();
    }
}
