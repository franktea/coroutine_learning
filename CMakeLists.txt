cmake_minimum_required(VERSION 3.5)

project(coroutine_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(coroutine_learning main.cpp)
add_executable(co_vs_callback co_vs_callback.cpp)
add_executable(entry_function entry_function.cpp)
