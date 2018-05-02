add_executable(test_udp_server
    server.cpp
)

target_include_directories(test_udp_server PUBLIC tinytcp)
target_link_libraries(test_udp_server tinytcp)
