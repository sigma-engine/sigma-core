cmake_minimum_required(VERSION 3.2)
project(shader-compiler)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(shader-compiler ${SOURCES} ${HEADERS})

target_include_directories(shader-compiler PUBLIC include)

target_link_libraries(shader-compiler engine pthread ${CONAN_LIBS})

