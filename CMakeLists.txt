cmake_minimum_required(VERSION 3.25)
project(VoxelEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)