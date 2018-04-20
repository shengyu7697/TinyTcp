add_executable(test_tcp_client
    client.cpp
    ../TinyTcp/TinyTcpClient.cpp
    ../TinyTcp/SocketUtil.cpp
)

target_include_directories(test_tcp_client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../TinyTcp")
if(WIN32)
target_link_libraries(test_tcp_client Ws2_32)
else()
target_link_libraries(test_tcp_client pthread)
endif()