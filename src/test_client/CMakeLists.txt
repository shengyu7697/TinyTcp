add_executable(test_client
    client.cpp
    ../TinyTcp/TinyTcpClient.cpp
    ../TinyTcp/SocketUtil.cpp
)

target_include_directories(test_client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../TinyTcp")
if(WIN32)
target_link_libraries(test_client Ws2_32)
else()
target_link_libraries(test_client pthread)
endif()