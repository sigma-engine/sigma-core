cmake_minimum_required(VERSION 2.8.12)
project(sdl_window)

set(SOURCES
	src/window.cpp)

set(HEADERS
	include/window.hpp)

add_library(sdl_window SHARED ${SOURCES} ${HEADERS})

target_include_directories(sdl_window
	PUBLIC
	include)

target_compile_definitions(sdl_window
PUBLIC
	-D$<$<CONFIG:Debug>:sdl_window_DEBUG>
PRIVATE
	-Dsdl_window_EXPORT)

if(NOT MSVC)
	target_compile_options(sdl_window PUBLIC -std=c++14 -pedantic)
endif()

target_link_libraries(sdl_window engine SDL2)


set_target_properties(sdl_window
			PROPERTIES
			PREFIX ""
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
