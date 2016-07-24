cmake_minimum_required(VERSION 3.2)
project(model-compiler)

set(SOURCES
    src/assimp_converter.hpp
    src/assimp_converter.cpp
    src/main.cpp
)

set(HEADERS
)

add_executable(model-compiler ${SOURCES} ${HEADERS})

target_include_directories(model-compiler PUBLIC include)

target_link_libraries(model-compiler engine assimp pthread ${CONAN_LIBS})
