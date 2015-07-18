cmake_minimum_required(VERSION 3.1.1)
project(Prime)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES prime.cpp)
add_executable(Prime ${SOURCE_FILES})