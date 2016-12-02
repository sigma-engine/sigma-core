cmake_minimum_required(VERSION 3.2)
project(screflect)

set(SOURCES
    src/main.cpp
    src/meta_data_parser.cpp
    src/meta_data_indexer.cpp
)

set(HEADERS
    include/meta_data_parser.hpp
    include/meta_data_indexer.hpp
)

add_executable(screflect ${SOURCES} ${HEADERS})
target_include_directories(screflect PUBLIC include ../../include)
target_link_libraries(screflect pthread ${CONAN_LIBS})
