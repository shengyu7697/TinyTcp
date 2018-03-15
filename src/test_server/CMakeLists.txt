add_executable(test_server
    server.cpp
    ../TinyTcp/TinyTcpServer.cpp
    ../TinyTcp/SocketUtil.cpp
)

target_include_directories(test_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../TinyTcp")
if(WIN32)
target_link_libraries(test_server Ws2_32)
else()
target_link_libraries(test_server pthread)
endif()