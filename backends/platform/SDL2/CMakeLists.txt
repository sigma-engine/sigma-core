cmake_minimum_required(VERSION 3.2)
project(ssdl2_platform)

set(SOURCES
    src/window.cpp
)

set(HEADERS
    include/window.hpp
)

add_library(ssdl2_platform STATIC ${SOURCES} ${HEADERS})
target_include_directories(ssdl2_platform PUBLIC include ../../../include)
target_compile_definitions(ssdl2_platform PUBLIC -D$<$<CONFIG:Debug>:SIGMA_SDL2_PLATFORM_DEBUG>)
target_link_libraries(ssdl2_platform sigma-engine pthread ${CONAN_LIBS})
