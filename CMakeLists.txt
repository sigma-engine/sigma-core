cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
include(cmake_utils)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(cppbr/math)
add_subdirectory(cppbr/opengl)

add_subdirectory(thirdparty)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(samples)
