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

void onRecv(int session, const char *buf, int len)
{
	printf("session=%d, len=%d, buf=%s\n", session, len, buf);
}

int main(int argc, char* argv[])
{
    /*if (argc < 2) {
        fprintf(stderr, "Usage: %s port_number\n", argv[0]);
        exit(1);
    }*/

	int port = 7000;
	printf("start server, bind on %d\n", port);

	TinyTcpServer server;
    server.setOnConnectCB(onConnect);
    server.setOnDisconnectCB(onDisconnect);
    server.setOnRecvCB(onRecv);
	server.start(port);

	char msg[256];
	while(gRunning)
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
			gRunning = false;
		} else {
			//server.send(msg, (int)strlen(msg));
			//server.send(1, msg, (int)strlen(msg)); // send to connection i
			server.sendAll(msg, (int)strlen(msg)); // send to all connection
		}
	}

	printf("end of process.\n");

    return 0;
}
