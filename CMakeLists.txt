cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

set(GCC_DEFINED_FLAGS "-Wall -pedantic -Werror")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_DEFINED_FLAGS}")

add_executable(Test main.cpp)
