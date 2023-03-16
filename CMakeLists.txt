cmake_minimum_required(VERSION 3.5)

project(coroutine_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(coroutine_learning main.cpp)
add_executable(co_vs_callback co_vs_callback.cpp)
add_executable(entry_function entry_function.cpp)
add_executable(timer_scheduler timer_scheduler.cpp)

