cmake_minimum_required(VERSION 3.15)
project(generator_project CXX)

#find_package(SDL3 CONFIG REQUIRED)

#-------------- Asset generation ----------------

add_executable(generator generator.cpp)
set_property(TARGET generator PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp")

# Define the files created by the asset generator
set(ASSET_OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/stuff.h")
set(ASSET_OUTPUT_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/stuff.cpp")

file(GLOB ASSET_STUFF "${CMAKE_CURRENT_SOURCE_DIR}/*")

# Define the command to run the generator
add_custom_command(
    OUTPUT ${ASSET_OUTPUT_HEADER} ${ASSET_OUTPUT_SOURCE}
    COMMAND $<TARGET_FILE:generator> ${ASSET_STUFF}
    DEPENDS generator ${ASSET_STUFF}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BIN_DIR}
)



add_executable(demo
    main.cpp
    ${ASSET_OUTPUT_SOURCE}
)

if (MSVC)
    # /W4 is the second-highest warning level for MSVC, catching most bugs without noise.
    target_compile_options(demo PRIVATE /W4)
else()
    target_compile_options(demo PRIVATE -Wall -Wpedantic)
endif ()

target_include_directories(demo PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(demo PRIVATE -g -fsanitize=address)
#target_link_options(demo PRIVATE -fsanitize=address)

#target_link_libraries(demo PRIVATE
#    SDL3::SDL3
#)