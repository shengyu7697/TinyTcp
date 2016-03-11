#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256

class TCPClient {
    public:
        TCPClient(){}
        ~TCPClient(){}

        int createSocket()
        {
            sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (sock_fd == -1) {
                perror("Socket creation");
                return -1;
            }

            return sock_fd;
        }

        int connect(char *ip_address, int port)
        {
            struct sockaddr_in serv_name;
            int status;

            // server address
            serv_name.sin_family = AF_INET;
            inet_aton(ip_address, &serv_name.sin_addr);
            serv_name.sin_port = htons(port);

            // connect to the server
            status = ::connect(sock_fd, (struct sockaddr *)&serv_name, sizeof(serv_name));
            if (status == -1) {
                perror("Connection error");
                return -1;
            }

            return status;
        }

        int mainLoop()
        {
            int max_fd;
            int nbytes;
            int ret;
            fd_set read_fds_master, read_fds;

            // set up variables for select
            max_fd = sock_fd;
            FD_ZERO(&read_fds_master);
            FD_SET(STDIN_FILENO, &read_fds_master);
            FD_SET(sock_fd, &read_fds_master);

            while(1) {
                // copy fd set
                read_fds = read_fds_master;

                // check to see if we can read from STDIN or sock_fd
                ret = select(max_fd+1, &read_fds, NULL, NULL, NULL);
                if (ret == -1) {
                    perror("Select");
                    return -1;
                } else if (ret == 0) {
                    printf("Select timeout\n");
                    continue;
                }

                // get input from STDIN and send message to server
                if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                    nbytes = read(STDIN_FILENO, buf, sizeof(buf));
                    if (nbytes > 0 && buf[0] != '\n') {
                        // place a null terminator at the end of the string
                        buf[nbytes-1] = '\0';
                        sendMessage(sock_fd, buf, nbytes);
                    }
                }

                // recevie message from server
                if (FD_ISSET(sock_fd, &read_fds)) {
                    memset(buf, 0, sizeof(buf));
                    nbytes = read(sock_fd, buf, sizeof(buf));
                    if (nbytes == 0) {
                        // connection closed
                        printf("server disconnect\n");
                        break;
                    } else {
                        receiveMessage(buf);
                    }
                }
            }
            close(sock_fd);

            return 0;
        }

        void sendMessage(int sock_fd, char *buf, int count)
        {
            write(sock_fd, buf, count);
        }

        void receiveMessage(char *buf)
        {
            printf("%s\n", buf);
        }

        //void setSendMessageCallback();
        //void setReceiveMessageCallback();

    private:
        int sock_fd;
        char buf[MAX_BUF];
};

void sendMessage(int sock_fd, char *buf, int count)
{
    write(sock_fd, &buf, count);
}

void receiveMessage(char *buf)
{
    printf("%s\n", buf);
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s ip_address port_number\n", argv[0]);
        exit(1);
    }

    TCPClient client;

    if (-1 == client.createSocket()) {
        exit(1);
    }

    if (-1 == client.connect(argv[1], atoi(argv[2]))) {
        exit(1);
    }

    //client.setSendMessageCallback(sendMessage);
    //client.setReceiveMessageCallback(receiveMessage);
    client.mainLoop();

    return 0;
}
