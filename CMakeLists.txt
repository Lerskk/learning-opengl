cmake_minimum_required(VERSION 3.10)
project(hello-triangle)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(glad glad/src/gl.c)
include_directories(glad/include)

find_package(glfw3 REQUIRED)

add_executable(hello-triangle main.c)
target_link_libraries(hello-triangle glfw glad m)
