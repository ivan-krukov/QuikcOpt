cmake_minimum_required(VERSION 3.0)

project(QuickOpts)

set(CMAKE_CXX_STANDARD 11)

file(GLOB EXAMPLE "src/example.cpp")
include_directories("include/")

# Example target
add_executable(example ${EXAMPLE})
