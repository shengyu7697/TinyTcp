project(TinyTcp)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(MSVC)
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "-W -Wall -O3 -std=c++11")
endif()

add_subdirectory(src/TinyTcp)
add_subdirectory(src/test_tcp_server)
add_subdirectory(src/test_tcp_client)
add_subdirectory(src/test_udp_server)
add_subdirectory(src/test_udp_client)