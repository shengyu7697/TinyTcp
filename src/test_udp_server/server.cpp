#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "TinyUdpServer.h"
#include "TimeUtil.h"

using namespace std;

void onRecv(const char *buf, int len)
{
    printf("Client: %s\n", buf);
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

    TinyUdpServer us;
    us.setOnRecv(onRecv);
    us.start(port);

    char msg[256];
    while (us.isRunning())
    {
        cin >> msg;
        if (strcmp(msg, "q") == 0) {
            us.stop();
        } /*else {
            if (server.isConnected()) {
                //us.send(msg, (int)strlen(msg)); // send to connection 0 // FIXME send to cur connection
                //us.send(1, msg, (int)strlen(msg)); // send to connection i
                us.sendAll(msg, (int)strlen(msg)); // send to all connection
            }
        }*/
        //sleep_ms(1000 * 1);
    }

    printf("end of process.\n");

    return 0;
}
