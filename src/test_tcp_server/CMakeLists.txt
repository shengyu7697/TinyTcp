add_executable(test_tcp_server
    server.cpp
)

target_include_directories(test_tcp_server PUBLIC tinytcp)
target_link_libraries(test_tcp_server tinytcp)
