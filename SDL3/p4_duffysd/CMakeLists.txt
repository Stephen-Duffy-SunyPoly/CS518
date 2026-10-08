cmake_minimum_required(VERSION 4.0)
project(p4_duffysd)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

find_package(SDL3 CONFIG QUIET)
find_package(SDL3_image CONFIG REQUIRED)

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

# asset compiler
add_executable(compile_resource compile_resource.cpp)
set_property(TARGET compile_resource PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp")
set(ASSET_OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/assets.h")
set(ASSET_OUTPUT_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/assets.c")
file(GLOB ASSET_STUFF "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")

add_custom_command(
        OUTPUT ${ASSET_OUTPUT_HEADER} ${ASSET_OUTPUT_SOURCE}
        COMMAND $<TARGET_FILE:compile_resource> ${ASSET_STUFF}
        DEPENDS compile_resource ${ASSET_STUFF}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BIN_DIR}
)

#regular application stuff
add_executable(p4_duffysd main.cpp frameRate.cpp color.c ${ASSET_OUTPUT_SOURCE})

if (MSVC)
    # /W4 is the second-highest warning level for MSVC
    target_compile_options(p4_duffysd PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(p4_duffysd PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_include_directories(p4_duffysd PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${sdl3_gfx_SOURCE_DIR})

target_link_libraries(SDL3_gfx_Static PRIVATE SDL3::SDL3)

target_link_libraries(p4_duffysd PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_gfx_Static)
