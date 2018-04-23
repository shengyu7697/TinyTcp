#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyUdpClient.h"
#include "TimeUtil.h"

using namespace std;

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

    TinyUdpClient uc;
    uc.start(hostname, port);

    char msg[256];
    while (uc.isRunning())
    {
        /*cin >> msg;
        if (strcmp(msg, "q") == 0) {
            uc.stop();
        } else {
            if (uc.isConnected()) {
                uc.send(msg, (int)strlen(msg));
            }
        }*/

        strcpy(msg, "Hello Wrold!");
        uc.send(msg, (int)strlen(msg));
        printf("send %s ...\n", msg);

        sleep_ms(1000 * 1);
    }

    printf("end of process.\n");

    return 0;
}
