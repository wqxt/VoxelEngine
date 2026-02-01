cmake_minimum_required(VERSION 4.2.0)
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

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glfw glm)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)

set(GLAD_SOURCES
    libs/glad/src/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SOURCES})

file(GLOB SHADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.frag"
)
add_custom_target(Shaders SOURCES ${SHADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
    ${glm_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)