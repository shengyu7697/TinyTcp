#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyTcpClient.h"

using namespace std;

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
	printf("Server: %s\n", buf);
}

int main(int argc, char* argv[])
{
	string hostname = "127.0.0.1";
	int port = 7000;

	if (argc > 3) {
		fprintf(stderr, "Usage: %s ip_address port_number\n", argv[0]);
		exit(1);
	} else if (argc == 3) {
		hostname = argv[1];
		port = atoi(argv[2]);
	}

	printf("start client, connect to %s:%d\n", hostname.c_str(), port);

	TinyTcpClient tc;
	tc.setOnConnect(onConnect);
	tc.setOnDisconnect(onDisconnect);
	tc.setOnRecv(onRecv);
	tc.start(hostname, port);

	char msg[256];
	while (tc.isRunning())
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
		    tc.stop();
		} else {
			if (tc.isConnected()) {
			    tc.send(msg, (int)strlen(msg));
			}
		}
	}

	printf("end of process.\n");

    return 0;
}
