#include "TinyUdpClient.h"
#include "SocketUtil.h"
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TinyUdpClient::TinyUdpClient() 
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[TinyTcp] WSAStartup failed, error: %d\n", WSAGetLastError());
    }
#endif
}

TinyUdpClient::~TinyUdpClient() 
{
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

int TinyUdpClient::send(const char *data, int size)
{
    int len = sendto(mSocket, data, size, 0,
        (struct sockaddr*)&mSrvAddr, sizeof(mSrvAddr));

    if (len < 0) {
        printf("[TinyTcp] Send failed, error:\n"); // FIXME getLastError()
    }
    return len;
}

int TinyUdpClient::start(const std::string &hostname, int port)
{
    int s = createUdpSocket();
    if (s < 0)
        return -1;

    // server address
    mSrvAddr.sin_family = AF_INET;
    inet_pton(AF_INET, hostname.c_str(), &(mSrvAddr.sin_addr));
    mSrvAddr.sin_port = htons(port);

    mSocket = s;
    mHostname = hostname;
    mPort = port;
    mRunning = true;

    //mThread = std::thread(&TinyUdpClient::run, this);
    return 1;
}

void TinyUdpClient::run()
{
}

void TinyUdpClient::stop()
{
    closeSocket(mSocket);
}

