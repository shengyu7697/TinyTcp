#include "TinyUdpClient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

TinyUdpClient::TinyUdpClient() 
{
}

TinyUdpClient::~TinyUdpClient() 
{
}

int TinyUdpClient::send(const char *data, int size)
{
    sendto(mSocket, data, size, 0,
        (struct sockaddr*)&mSrvAddr, sizeof(mSrvAddr));
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

int TinyUdpClient::start(const std::string &hostname, int port)
{
    int s = createUdpSocket();
    if (s < 0)
        return -1;

    // server address
    mSrvAddr.sin_family = AF_INET;
    inet_aton(hostname.c_str(), &mSrvAddr.sin_addr);
    mSrvAddr.sin_port = htons(port);

    mSocket = s;
    mHostname = hostname;
    mPort = port;
    mRunning = true;

    //mThread = std::thread(&TinyUdpClient::run, this);
}

void TinyUdpClient::run()
{
}


void TinyUdpClient::stop()
{
    close(mSocket);
}

