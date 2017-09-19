cmake_minimum_required(VERSION 3.8)

project(meshSimp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
file(GLOB SOURCE_FILES *.cpp)

add_executable(meshSimp ${SOURCE_FILES})

