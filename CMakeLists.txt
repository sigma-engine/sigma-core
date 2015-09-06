cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_subdirectory(sigmafive-engine)
add_subdirectory(sigmafive-editor)
add_subdirectory(sigmafive-samples)
