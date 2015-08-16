cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

if(UNIX)
    set(SIGMAFIVE_OS_UNIX TRUE)
    add_definitions("-DSIGMAFIVE_OS_UNIX")
    if(APPLE)
        set(SIGMAFIVE_OS_APPLE TRUE)
        add_definitions("-DSIGMAFIVE_OS_APPLE")
    endif()
elseif(WIN32)
    set(SIGMAFIVE_OS_WIN32 TRUE)
    add_definitions("-DSIGMAFIVE_OS_WIN32")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(SIGMAFIVE_CLANG TRUE)
    add_definitions("-DSIGMAFIVE_CLANG")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(SIGMAFIVE_GNU_GCC TRUE)
    add_definitions("-DSIGMAFIVE_GNU_GCC")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(SIGMAFIVE_MSVC TRUE)
    add_definitions("-DSIGMAFIVE_MSVC")
endif()

if(SIGMAFIVE_CLANG OR SIGMAFIVE_GNU_GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
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
add_subdirectory(samples)
