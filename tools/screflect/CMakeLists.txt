cmake_minimum_required(VERSION 3.2)
project(screflect)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(screflect ${SOURCES} ${HEADERS})
target_link_libraries(screflect pthread ${CONAN_LIBS})
