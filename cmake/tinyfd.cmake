add_library(tinyfd INTERFACE)
set(TINYFILEDIALOGS_DIR ${CMAKE_SOURCE_DIR}/external/tinyfiledialogs)
target_sources(tinyfd INTERFACE
        ${TINYFILEDIALOGS_DIR}/tinyfiledialogs.c
)
target_include_directories(tinyfd INTERFACE
        ${TINYFILEDIALOGS_DIR}
)