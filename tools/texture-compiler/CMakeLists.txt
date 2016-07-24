cmake_minimum_required(VERSION 3.2)
project(texture-compiler)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(texture-compiler ${SOURCES} ${HEADERS})

target_include_directories(texture-compiler PUBLIC include)

target_link_libraries(texture-compiler engine freeimage jpeg mng openexr openjpeg png rawlite tiff zlib ${CONAN_LIBS})

