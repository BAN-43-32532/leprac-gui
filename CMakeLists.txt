cmake_minimum_required(VERSION 3.30)
project(leprac)
set(CMAKE_CXX_STANDARD 23)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
add_subdirectory(${EXTERNAL_DIR}/ImGuiFileDialog)
add_subdirectory(${EXTERNAL_DIR}/magic_enum)


add_executable(leprac ${CMAKE_SOURCE_DIR}/main.cpp)
file(GLOB_RECURSE SOURCES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.h
)
target_sources(leprac PRIVATE ${SOURCES})
target_include_directories(leprac PRIVATE external)

include(${CMAKE_SOURCE_DIR}/cmake/imgui.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tinyfd.cmake)

set(SDL3_DIR ${EXTERNAL_DIR}/SDL3/cmake)
find_package(SDL3 REQUIRED)

#target_sources(leprac PRIVATE ${EXTERNAL_DIR}/magic_enum/include)
target_include_directories(leprac PRIVATE ${EXTERNAL_DIR}/magic_enum/include)
#target_include_directories(leprac PUBLIC external/imgui)
#target_compile_definitions(leprac PUBLIC IMGUI_INCLUDE="external/imgui/imgui.h")

target_link_libraries(leprac PRIVATE
        imgui
        SDL3::SDL3
        tinyfd
        ImGuiFileDialog
        magic_enum::magic_enum
)
