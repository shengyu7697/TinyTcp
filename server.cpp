#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256

class TCPServer
{
    public:
        TCPServer(){}

        ~TCPServer(){}

        int createSocket()
        {
            // create a socket
            sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (sock_fd == -1) {
                perror("Socket creation error");
                return -1;
            }

            return sock_fd;
        }

        int bind(int port)
        {
            int on = 1;
            struct sockaddr_in my_name;
            int status;

            // for "Address already in use" error message
            if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == -1) {
                perror("Setsockopt error");
                return -1;
            }

            // server address
            my_name.sin_family = AF_INET;
            my_name.sin_addr.s_addr = INADDR_ANY;
            my_name.sin_port = htons(port);

            status = ::bind(sock_fd, (struct sockaddr *)&my_name, sizeof(my_name));
            if (status == -1) {
                perror("Binding error");
                return -1;
            }
            printf("server ip=%s:%d\n", inet_ntoa(my_name.sin_addr), port);

            return status;
        }

        int listen(int backlog)
        {
            int status;

            status = ::listen(sock_fd, backlog);
            if (status == -1) {
                perror("Listening error");
                return -1;
            }

            return status;
        }

        int mainLoop()
        {
            struct sockaddr_in peer_name;
            socklen_t addrlen;
            int new_fd, max_fd;
            int nbytes;
            int i, j, ret;
            fd_set read_fds_master, read_fds;

            addrlen = sizeof(peer_name);
            // set up variables for select
            max_fd = sock_fd;
            FD_ZERO(&read_fds_master);
            FD_SET(sock_fd, &read_fds_master);

            while(1)
            {
                // copy fd set
                read_fds = read_fds_master;
                ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
                if (ret == -1) {
                    perror("Select");
                    return -1;
                } else if (ret == 0) {
                    printf("Select timeout\n");
                    continue;
                }

                for (i = 0; i < max_fd+1; i++) {
                    if (FD_ISSET(i, &read_fds)) {
                        if (i == sock_fd) {
                            // wait for a connection
                            new_fd = accept(sock_fd, (struct sockaddr *)&peer_name, &addrlen);
                            printf("accecpt fd%d ip=%s:%d\n", new_fd,
                                    inet_ntoa(peer_name.sin_addr), ntohs(peer_name.sin_port));

                            if (new_fd == -1) {
                                perror("Wrong connection");
                                exit(1);
                            }

                            // add to fd set
                            FD_SET(new_fd, &read_fds_master);
                            if (new_fd > max_fd) {
                                max_fd = new_fd;
                            }
                        } else {
                            // already connected socket
                            memset(buf, 0, sizeof(buf));
                            nbytes = read(i, buf, sizeof(buf));
                            if (nbytes == 0) {
                                // connection closed
                                close(i);
                                printf("client fd%d disconnect\n", i);

                                // remove from fd set
                                FD_CLR(i, &read_fds_master);
                                break;
                            } else {
                                receiveMessage(buf);
                            }

                            // send message to connected clients
                            for (j = 0; j < max_fd+1; j++) {
                                if (FD_ISSET(j, &read_fds_master)) {
                                    if (j != sock_fd && j != i) {
                                        sendMessage(j, buf, nbytes);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        void receiveMessage(char *buf)
        {
            printf("%s\n", buf);
        }

        void sendMessage(int sock_fd, char *buf, int count)
        {
            write(sock_fd, buf, count);
        }

    private:
        int sock_fd;
        char buf[MAX_BUF];
};

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    TCPServer server;

    if (-1 == server.createSocket()) {
        exit(1);
    }

    if (-1 == server.bind(atoi(argv[1]))) {
        exit(1);
    }

    if (-1 == server.listen(5)) {
        exit(1);
    }

    server.mainLoop();

    return 0;
}
