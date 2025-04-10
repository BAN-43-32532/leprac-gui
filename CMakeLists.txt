cmake_minimum_required(VERSION 3.30)
project(leprac)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_executable(leprac main.cpp)
target_sources(leprac PRIVATE ${SOURCES})
target_include_directories(leprac PRIVATE external)

set(TEST_DIR tests)
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp tests/*.h)
add_executable(test_config ${TEST_DIR}/test_config.cpp)
target_include_directories(test_config PRIVATE external)