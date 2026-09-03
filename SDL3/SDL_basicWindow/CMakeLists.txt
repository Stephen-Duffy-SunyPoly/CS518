cmake_minimum_required(VERSION 3.22)
project(SDL_basicWindow)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL_TESTS OFF CACHE BOOL "Disable SDL test suite" FORCE)

# screw all that complicated install and command line args stuf, this is way easier
fetchcontent_declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-3.4.14
)

fetchcontent_makeavailable(SDL3)

add_executable(SDL_basicWindow main.cpp)

target_link_libraries(SDL_basicWindow PRIVATE SDL3::SDL3)