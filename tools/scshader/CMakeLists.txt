cmake_minimum_required(VERSION 3.2)
project(scshader)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(scshader ${SOURCES} ${HEADERS})

target_include_directories(scshader PUBLIC include)

target_link_libraries(scshader sigma-core)
