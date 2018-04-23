add_executable(test_udp_server
    server.cpp
    ../TinyTcp/TinyUdpServer.cpp
    ../TinyTcp/SocketUtil.cpp
    ../TinyTcp/TimeUtil.cpp
)

target_include_directories(test_udp_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../TinyTcp")
if(WIN32)
target_link_libraries(test_udp_server Ws2_32)
else()
target_link_libraries(test_udp_server pthread)
endif()