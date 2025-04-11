#add_library(test_common INTERFACE)
#target_include_directories(test_common INTERFACE
#        ${CMAKE_SOURCE_DIR}/external
#)
#target_link_libraries(test_common INTERFACE
#        ${CMAKE_SOURCE_DIR}/external/SDL3/lib/libSDL3.dll.a
#)
#target_sources(test_common PRIVATE ${SOURCES})

#file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#foreach(test_src ${TEST_SOURCES})
#    get_filename_component(test_name ${test_src} NAME_WE)
#    add_executable(${test_name} ${test_src})
#    target_link_libraries(${test_name} PRIVATE ${YOUR_LIBS})
#    target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
#endforeach()
add_executable(test_config ${CMAKE_CURRENT_SOURCE_DIR}/test_config.cpp)
set(TINYFILEDIALOGS_DIR ${CMAKE_SOURCE_DIR}/external/tinyfiledialogs)
target_sources(test_config PRIVATE
        ${TINYFILEDIALOGS_DIR}/tinyfiledialogs.c
)
target_include_directories(test_config PRIVATE
        ${TINYFILEDIALOGS_DIR}
)