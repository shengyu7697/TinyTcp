#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyUdpClient.h"
#include "TimeUtil.h"

using namespace std;

void testStdin(const std::string &hostname, const int &port)
{
	TinyUdpClient uc;
	uc.start(hostname, port);

	char msg[256];
	while (uc.isRunning())
	{
		cin >> msg;
		if (strcmp(msg, "q") == 0) {
			uc.stop();
		} else {
			uc.send(msg, (int)strlen(msg));
		}
	}
}

void testPacketLost(const std::string &hostname, const int &port)
{
	TinyUdpClient uc;
	uc.start(hostname, port);

	int count = 1;

	char msg[256];
	while (uc.isRunning())
	{
		sprintf(msg, "Hello Wrold! %d", count);
		uc.send(msg, (int)strlen(msg));
		printf("send %s ...\n", msg);

		if (count == 1000) {
			uc.stop();
			break;
		}
		count++;

		//sleep_ms(1000 * 1);
	}
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

    //testStdin(hostname, port);
    testPacketLost(hostname, port);

    printf("end of process.\n");

    return 0;
}
