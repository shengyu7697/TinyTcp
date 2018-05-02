add_library(tinytcp STATIC
    TinyTcpClient.h
    TinyTcpClient.cpp
    TinyTcpServer.h
    TinyTcpServer.cpp
    TinyUdpClient.h
    TinyUdpClient.cpp
    TinyUdpServer.h
    TinyUdpServer.cpp
    SocketUtil.h
    SocketUtil.cpp
    TimeUtil.h
    TimeUtil.cpp
)

target_include_directories(tinytcp PUBLIC ./)
if(WIN32)
target_link_libraries(tinytcp Ws2_32)
else()
target_link_libraries(tinytcp pthread)
endif()