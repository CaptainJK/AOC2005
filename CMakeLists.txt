cmake_minimum_required(VERSION 3.20)
project(aoc2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(CURL REQUIRED)

add_executable(aoc main.cpp)
target_link_libraries(aoc PUBLIC CURL::libcurl)
