cmake_minimum_required(VERSION 3.4)

project(meshSimp)
set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc/7.2.0/bin/g++-7)
set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(~/include/)
file(GLOB header *.h)
set(SOURCE_FILES main.cpp)
file(GLOB SOURCE_FILES *.cpp)

add_executable(meshSimp ${SOURCE_FILES})

