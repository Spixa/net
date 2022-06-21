cmake_minimum_required(VERSION 3.1)
project(vm VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  ${CMAKE_SOURCE_DIR}/src/main.cpp
)
add_executable(vm ${SOURCES})