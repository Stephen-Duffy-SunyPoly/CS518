cmake_minimum_required(VERSION 3.22)
project(SDL_render CXX C)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

find_package(SDL3 CONFIG QUIET)
find_package(SDL3_image CONFIG REQUIRED)


if(NOT SDL3_FOUND)
    message("SDL3 not found installed on the system, downloading...")
    set(SDL_TESTS OFF CACHE BOOL "Disable SDL test suite" FORCE)
    # screw all that complicated install and command line args stuf, this is way easier
    FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG release-3.4.14
    )

    FetchContent_MakeAvailable(SDL3)
endif ()

FetchContent_Declare(
        SDL3_GFX
        GIT_REPOSITORY https://github.com/sabdul-khabir/SDL3_gfx
        GIT_TAG v1.0.1
)

FetchContent_MakeAvailable(SDL3_GFX)

add_executable(SDL_render main.cpp
        color.c
        color.h
        frameRate.cpp
        frameRate.hpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(SDL_render PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(SDL_render PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(SDL_render PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_GFX_Static)