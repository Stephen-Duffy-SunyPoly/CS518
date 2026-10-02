cmake_minimum_required(VERSION 3.22)
project(compile_resource)

set(CMAKE_CXX_STANDARD 20)

add_executable(compile_resource main.cpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(compile_resource PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(compile_resource PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(test test.cpp assets.c)