cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

if(NOT MSVC) #TODO others??
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
#include(cmake_utils)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(cppbr/math)
add_subdirectory(cppbr/opengl)

add_subdirectory(thirdparty)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(samples)

add_subdirectory(plugins/opengl_context)
add_subdirectory(plugins/sdl_window)
add_subdirectory(plugins/assimp_import_plugin)
