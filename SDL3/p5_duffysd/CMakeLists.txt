cmake_minimum_required(VERSION 4.0)
project(p5_duffysd)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

find_package(SDL3 CONFIG QUIET)

if(NOT SDL3_FOUND)
    message("SDL3 not found installed on local system, downloading...")
    fetchcontent_declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG release-3.4.14
    )

    fetchcontent_makeavailable(SDL3)
endif ()

if (NOT SDL3_LIB)
    set(SDL3_LIB SDL3::SDL3)
endif ()

FetchContent_Declare(
        SDL3_GFX
        GIT_REPOSITORY https://github.com/sabdul-khabir/SDL3_gfx
        GIT_TAG v1.0.1
)

FetchContent_MakeAvailable(SDL3_GFX)

#regular application stuff
add_executable(p5_duffysd main.cpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC
    target_compile_options(p5_duffysd PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(p5_duffysd PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_include_directories(p5_duffysd PRIVATE ${sdl3_gfx_SOURCE_DIR})

target_link_libraries(SDL3_gfx_Static PRIVATE SDL3::SDL3)

target_link_libraries(p5_duffysd PRIVATE SDL3::SDL3 SDL3_gfx_Static)
