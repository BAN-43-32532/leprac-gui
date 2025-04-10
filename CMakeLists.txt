cmake_minimum_required(VERSION 3.30)
project(leprac LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)

FetchContent_MakeAvailable(imgui)

add_executable(leprac main.cpp)

target_link_libraries(leprac PRIVATE imgui)