cmake_minimum_required(VERSION 3.2)
project(scmodel)

set(SOURCES
    src/assimp_converter.hpp
    src/assimp_converter.cpp
    src/main.cpp
)

set(HEADERS
)

add_executable(scmodel ${SOURCES} ${HEADERS})

target_include_directories(scmodel PUBLIC include)

target_link_libraries(scmodel sigma-engine assimp pthread ${CONAN_LIBS})
