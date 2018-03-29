#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyTcpServer.h"

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
	printf("Client %d: %s\n", session, buf);
}

int main(int argc, char* argv[])
{
	int port = 7000;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	} else if (argc == 2) {
		port = atoi(argv[1]);
	}

	printf("start server, bind on %d\n", port);

	TinyTcpServer server;
    server.setOnConnect(onConnect);
    server.setOnDisconnect(onDisconnect);
    server.setOnRecv(onRecv);
	server.start(port);

	char msg[256];
	while (server.isRunning())
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
			server.stop();
		} else {
			//server.send(msg, (int)strlen(msg)); // send to connection 0 // FIXME send to cur connection
			//server.send(1, msg, (int)strlen(msg)); // send to connection i
			server.sendAll(msg, (int)strlen(msg)); // send to all connection
		}
	}

	printf("end of process.\n");

    return 0;
}
