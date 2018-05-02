add_executable(test_tcp_client
    client.cpp
)

target_include_directories(test_tcp_client PUBLIC tinytcp)
target_link_libraries(test_tcp_client tinytcp)
