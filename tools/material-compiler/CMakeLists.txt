cmake_minimum_required(VERSION 3.2)
project(material-compiler)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(material-compiler ${SOURCES} ${HEADERS})

target_include_directories(material-compiler PUBLIC include)

target_link_libraries(material-compiler engine pthread ${CONAN_LIBS})

if(NOT MSVC)
    target_compile_options(material-compiler PUBLIC -std=c++14 -pedantic )
endif()
