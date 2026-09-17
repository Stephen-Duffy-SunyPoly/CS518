cmake_minimum_required(VERSION 3.22)
project(SDL_image)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG QUIET)

if(NOT SDL3_FOUND)
    include(FetchContent)
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


fetchcontent_makeavailable(SDL3)

add_executable(SDL_image main.cpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(SDL_image PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(SDL_image PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(SDL_image PRIVATE SDL3::SDL3)