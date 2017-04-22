cmake_minimum_required(VERSION 3.7)
project(TinyEmulator)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES tinytest.c main.c)
add_executable(TinyEmulator ${SOURCE_FILES})