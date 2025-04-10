cmake_minimum_required(VERSION 3.30)
project(leprac)

add_executable(leprac main.cpp)

target_link_libraries(leprac PRIVATE imgui)