#include "TinyUdpServer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

TinyUdpServer::TinyUdpServer() 
{
}

TinyUdpServer::~TinyUdpServer() 
{
}

void TinyUdpServer::setOnRecv(OnRecv onRecv)
{
    this->onRecv = onRecv;
}

int createUdpSocket()
{
    int s;
    // create a socket
    s = (int)socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        //perror("Socket creation error");
        return -1;
    }

    return s;
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
    int status, nbytes;
    int addrlen;

    // server address
    my_name.sin_family = AF_INET;
    my_name.sin_addr.s_addr = INADDR_ANY;
    my_name.sin_port = htons(mPort);

    status = bind(mSocket, (struct sockaddr*)&my_name, sizeof(my_name));

    addrlen = sizeof(cli_name);
    while (mRunning)
    {
        memset(buf, 0, sizeof(buf));
        nbytes = recvfrom(mSocket, buf, sizeof(buf), 0,
                (struct sockaddr*)&cli_name, (socklen_t *)&addrlen);

        printf("recv from %s:%d\n", inet_ntoa(cli_name.sin_addr), ntohs(cli_name.sin_port));
        printf("%s\n", buf);
    }

    mRunning = false;
}

void TinyUdpServer::stop()
{
    mRunning = false;
    close(mSocket);
}
