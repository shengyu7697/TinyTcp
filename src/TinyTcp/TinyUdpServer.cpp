#include "TinyUdpServer.h"
#include "SocketUtil.h"
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TinyUdpServer::TinyUdpServer() :
    onRecv(nullptr)
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("[TinyTcp] WSAStartup failed, error: %d\n", WSAGetLastError());
    }
#endif
}

TinyUdpServer::~TinyUdpServer() 
{
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void TinyUdpServer::setOnRecv(OnRecv onRecv)
{
    this->onRecv = onRecv;
}

int TinyUdpServer::start(int port)
{
    int s = createUdpSocket();
    if (s < 0)
        return -1;

    mSocket = s;
    mPort = port;
    mRunning = true;

    mThread = std::thread(&TinyUdpServer::run, this);
}

void TinyUdpServer::run()
{
    struct sockaddr_in my_name, cli_name;
    char buf[4096];
    int status;
    int addrlen;

    // server address
    my_name.sin_family = AF_INET;
    my_name.sin_addr.s_addr = INADDR_ANY;
    my_name.sin_port = htons(mPort);

    status = ::bind(mSocket, (struct sockaddr*)&my_name, sizeof(my_name));

    addrlen = sizeof(cli_name);
    while (mRunning)
    {
        memset(buf, 0, sizeof(buf));
        int len = recvfrom(mSocket, buf, sizeof(buf), 0,
                (struct sockaddr*)&cli_name, (socklen_t *)&addrlen);

        //printf("recv from %s:%d\n", inet_ntoa(cli_name.sin_addr), ntohs(cli_name.sin_port));
        //printf("%s\n", buf);

        // notify onRecv callback
        if (onRecv)
            onRecv(buf, len);
    }

    mRunning = false;
}

void TinyUdpServer::stop()
{
    mRunning = false;
    closeSocket(mSocket);
}
