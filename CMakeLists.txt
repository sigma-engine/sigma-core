cmake_minimum_required(VERSION 2.8.12)
project(sigma-five)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wno-unused-function")
endif()

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

#Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

#GoogleMock
set(gtest_force_shared_crt ON)
set(gtest_disable_pthreads ON)

add_subdirectory(${THIRDPARTY_DIR}/googlemock ${CMAKE_BINARY_DIR}/thirdparty)
include_directories(SYSTEM ${THIRDPARTY_DIR}/googlemock/include ${THIRDPARTY_DIR}/googlemock/gtest/include)

add_subdirectory(engine)

add_subdirectory(editor)
