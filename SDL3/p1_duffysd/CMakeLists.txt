cmake_minimum_required(VERSION 4.0)
project(p1_duffysd)

find_package(SDL3 CONFIG QUIET)

if(NOT SDL3_FOUND)
    include(FetchContent)
    message("SDL3 not found installed on local system, downloading...")
    fetchcontent_declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL
            GIT_TAG release-3.4.14
    )

    fetchcontent_makeavailable(SDL3)
endif ()

add_executable(p1_duffysd main.cpp)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC
    target_compile_options(p1_duffysd PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(p1_duffysd PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(p1_duffysd PRIVATE SDL3::SDL3)