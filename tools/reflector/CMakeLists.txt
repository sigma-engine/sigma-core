cmake_minimum_required(VERSION 3.2)
project(reflector)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(reflector ${SOURCES} ${HEADERS})
target_link_libraries(reflector pthread ${CONAN_LIBS})

if(NOT MSVC)
    target_compile_options(reflector PUBLIC -std=c++14 -pedantic)
endif()
