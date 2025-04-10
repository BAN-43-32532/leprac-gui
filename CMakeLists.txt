cmake_minimum_required(VERSION 3.30)
project(leprac)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(leprac PRIVATE external)