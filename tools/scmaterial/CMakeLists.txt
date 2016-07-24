cmake_minimum_required(VERSION 3.2)
project(scmaterial)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

add_executable(scmaterial ${SOURCES} ${HEADERS})

target_include_directories(scmaterial PUBLIC include)

target_link_libraries(scmaterial sigma-engine pthread ${CONAN_LIBS})
