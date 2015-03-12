cmake_minimum_required(VERSION 2.8.12)
project(sigma-five)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
endif()

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

#Boost
find_package(Boost REQUIRED COMPONENTS serialization)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

add_subdirectory(engine)

add_subdirectory(editor)
