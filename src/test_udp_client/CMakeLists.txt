add_executable(test_udp_client
    client.cpp
)

target_include_directories(test_udp_client PUBLIC tinytcp)
target_link_libraries(test_udp_client tinytcp)
