cmake_minimum_required(VERSION 3.12)
project(chronal_calibration)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(solve "solution.cpp")
