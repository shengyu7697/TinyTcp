#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyTcpClient.h"

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
	//printf("session=%d, len=%d, buf=%s\n", session, len, buf);
	printf("%s\n", buf);
}

int main(int argc, char* argv[])
{
    /*if (argc < 3) {
        fprintf(stderr, "Usage: %s ip_address port_number\n", argv[0]);
        exit(1);
    }*/

	TinyTcpClient client;
	client.setOnConnectCB(onConnect);
	client.setOnDisconnectCB(onDisconnect);
	client.setOnRecvCB(onRecv);
	client.start("127.0.0.1", 7000);

	char msg[256];
	while(gRunning)
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
			gRunning = false;
		} else {
			client.send(msg, strlen(msg));
		}
	}

    return 0;
}
