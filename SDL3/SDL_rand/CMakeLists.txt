cmake_minimum_required(VERSION 3.22)
project(SDL_rand)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 CONFIG QUIET)

if(NOT SDL3_FOUND)
    message("SDL3 not found installed on the system, downloading...")
    set(SDL_TESTS OFF CACHE BOOL "Disable SDL test suite" FORCE)
    # screw all that complicated install and command line args stuf, this is way easier
    fetchcontent_declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG release-3.4.14
    )

    fetchcontent_makeavailable(SDL3)
endif ()

add_executable(SDL_rand main.cpp)
add_executable(bounce bounce.cpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(SDL_rand PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(SDL_rand PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(SDL_rand PRIVATE SDL3::SDL3)