cmake_minimum_required(VERSION 3.6)
project(InfiniVBBM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(vmmbfix vbbm.cpp UnbufferedFile.h UnbufferedFile.cpp)
add_executable(lockread lockread.cpp)
