cmake_minimum_required(VERSION 2.8.12)
project(sigma-five)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
endif()

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

#Boost
find_package(Boost REQUIRED COMPONENTS serialization)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

#SDL2
add_library(SDL2main STATIC IMPORTED)
set_target_properties(SDL2main PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX})

add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_IMPLIB ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories(SYSTEM ${THIRDPARTY_DIR}/SDL/include)

set(SRCS
    src/system/window.cpp
    src/main.cpp
)
set(HDRS
    include/math/mat2.hpp
    include/math/mat3.hpp
    include/math/mat4.hpp
    include/math/quaternion.hpp
    include/math/utils.hpp
    include/math/vec2.hpp
    include/math/vec3.hpp
    include/math/vec4.hpp

    include/system/window.hpp
)

include_directories(include)
add_executable(sigma-five ${SRCS} ${HDRS})
target_link_libraries(sigma-five ${Boost_LIBRARIES} SDL2 SDL2main)
