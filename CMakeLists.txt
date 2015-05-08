cmake_minimum_required(VERSION 3.1)
project(assignment_5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(assignment_5 ${SOURCE_FILES} src/Audio.h)
