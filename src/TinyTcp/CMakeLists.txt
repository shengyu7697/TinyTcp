set(FILES
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

if(MSVC)
else()
list(APPEND FILES
    NetworkUtil.h
    NetworkUtil.cpp
)
endif()

add_library(tinytcp STATIC ${FILES})

target_include_directories(tinytcp PUBLIC ./)
if(MSVC)
target_link_libraries(tinytcp Ws2_32)
else()
target_link_libraries(tinytcp pthread)
endif()