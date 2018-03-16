#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

int createSocket();
void closeSocket(int socket);
int getLastError();

#endif // SOCKETUTIL_H
