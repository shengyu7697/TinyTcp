#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyTcpServer.h"

using namespace std;

bool gRunning = true;

void onConnect(int session)
{
	printf("onConnect\n");
}

void onDisconnect(int session)
{
	printf("onDisconnect\n");
}

void onRecv(int socket, int session, const char *buf, int len)
{
	printf("socket=%d, session=%d, len=%d, buf=%s\n", socket, session, len, buf);
}

int main(int argc, char* argv[])
{
    /*if (argc < 2) {
        fprintf(stderr, "Usage: %s port_number\n", argv[0]);
        exit(1);
    }*/

	TinyTcpServer server;
    server.setOnConnectCB(onConnect);
    server.setOnDisconnectCB(onDisconnect);
    server.setOnRecvCB(onRecv);
	server.start(7000);

	char msg[256];
	while(gRunning)
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
			gRunning = false;
		} else {
			//server.send(msg, strlen(msg));
			//server.send(1, msg, strlen(msg)); // send to connection i
			server.sendAll(msg, strlen(msg)); // send to all connection
		}
	}

	cout << "end of process\n";

    return 0;
}
