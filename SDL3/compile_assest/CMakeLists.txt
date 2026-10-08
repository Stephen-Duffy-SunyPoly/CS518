cmake_minimum_required(VERSION 3.22)
project(compile_resource)

set(CMAKE_CXX_STANDARD 20)

add_executable(compile_resource main.cpp)
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


if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(compile_resource PRIVATE /W4)
else()
    # all other non Microsoft compilers
    target_compile_options(compile_resource PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(test test.cpp ${ASSET_OUTPUT_SOURCE})
target_include_directories(test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})