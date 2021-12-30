cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(fds)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -DDEBUG -fuse-ld=lld")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(test_big_integer test_big_integer.cpp list.h list_impl.h big_integer.h big_integer_impl.h)
target_link_libraries(test_big_integer ${CONAN_LIBS})