#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

int createSocket();
void closeSocket(int socket);
int acceptSocket(int socket);
int getLastError();

#define DEBUG 1
#if defined(DEBUG)
#if DEBUG >= 4 // filename:function:line
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] %s:%s():%d " fmt, \
    __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#elif DEBUG >= 3 // filename:function
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] %s:%s() " fmt, \
    __FILE__, __func__, ##__VA_ARGS__)
#elif DEBUG >= 2 // filename:line
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d " fmt, \
    __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#elif DEBUG >= 1 // function:line
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "[DEBUG] %s():%d " fmt, \
    __func__, __LINE__, ##__VA_ARGS__)
#endif
#else
#define DEBUG_PRINT(fmt, ...) // Don't do anything in release builds
#endif

#endif // SOCKETUTIL_H
