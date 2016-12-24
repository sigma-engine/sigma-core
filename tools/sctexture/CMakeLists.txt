cmake_minimum_required(VERSION 3.2)
project(sctexture)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(sctexture ${SOURCES} ${HEADERS})

target_include_directories(sctexture PUBLIC include)

target_link_libraries(sctexture sigma-core)
