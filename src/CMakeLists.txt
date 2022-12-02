cmake_minimum_required(VERSION 3.20)

project(aoc-2022)

set(BOOST_ROOT /usr/include/boost169)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-std=c++2a -ggdb -Wall)  # -Og makes it faster, but hard to debug

add_executable(aoc-2022 main.cpp runner.cpp week1.cpp)
