cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

#Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

#GoogleMock
set(gtest_force_shared_crt ON)
set(gtest_disable_pthreads ON)

add_subdirectory(${THIRDPARTY_DIR}/googlemock ${CMAKE_BINARY_DIR}/thirdparty)
include_directories(SYSTEM ${THIRDPARTY_DIR}/googlemock/include ${THIRDPARTY_DIR}/googlemock/gtest/include)

#SDL2
add_library(SDL2main STATIC IMPORTED)
set_target_properties(SDL2main PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2main${CMAKE_STATIC_LIBRARY_SUFFIX})

add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_IMPLIB ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_STATIC_LIBRARY_PREFIX}SDL2${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/SDL/lib/${CMAKE_SHARED_LIBRARY_PREFIX}SDL2${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories(SYSTEM ${THIRDPARTY_DIR}/SDL/include)

#OpenGL
find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})

#Generated opengl header and function pointer loader
find_program(LUA_EXECUTABLE
    NAMES lua Lua lua51 Lua51 lua52 Lua52
    DOC "lua command line client"
)
if(${LUA_EXECUTABLE} STREQUAL "LUA_EXECUTABLE-NOTFOUND")
    message(FATAL_ERROR "could not find lua")
else()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated)
    set(args ${THIRDPARTY_DIR}/.src/glloadgen/LoadGen.lua -style=noload_cpp -spec=gl -version=4.0 -profile=core core_4_0)
    execute_process(COMMAND
        ${LUA_EXECUTABLE} ${args}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated
        RESULT_VARIABLE RESULT
     )
     if(NOT ${RESULT} EQUAL 0)
        message(FATAL_ERROR "could not generate opengl headers.")
     endif()
endif()
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/generated)

set(SRCS
    src/sigmafive/application.cpp
    src/sigmafive/context.cpp
    src/sigmafive/object.cpp

    src/sigmafive/system/window.cpp
    src/sigmafive/system/resource.cpp
    src/sigmafive/system/resource_manager.cpp

    src/sigmafive/graphics/graphics_system.cpp
    src/sigmafive/graphics/static_mesh.cpp

    generated/gl_core_4_0.cpp
    src/sigmafive/graphics/opengl/buffer.cpp
    src/sigmafive/graphics/opengl/frame_buffer.cpp
    src/sigmafive/graphics/opengl/index_buffer.cpp
    src/sigmafive/graphics/opengl/material.cpp
    src/sigmafive/graphics/opengl/ogl_graphics_system.cpp
    src/sigmafive/graphics/opengl/program.cpp
    src/sigmafive/graphics/opengl/scene_renderer.cpp
    src/sigmafive/graphics/opengl/shader.cpp
    src/sigmafive/graphics/opengl/static_mesh.cpp
    src/sigmafive/graphics/opengl/static_mesh_manager.cpp
    src/sigmafive/graphics/opengl/texture_2d.cpp
    src/sigmafive/graphics/opengl/vertex_array.cpp
    src/sigmafive/graphics/opengl/vertex_buffer.cpp

    src/sigmafive/game/scene.cpp
    src/sigmafive/game/entity.cpp
    src/sigmafive/game/component.cpp
    src/sigmafive/game/entity_world.cpp
    src/sigmafive/game/entity_manager.cpp
    src/sigmafive/game/component_pool.cpp
    src/sigmafive/game/component_bitset_manager.cpp
    src/sigmafive/game/component_system.cpp
    src/sigmafive/game/component_manager.cpp
    src/sigmafive/game/transform_component.cpp
    src/sigmafive/game/static_mesh_component.cpp
    src/sigmafive/game/component_system_manager.cpp
    src/sigmafive/game/static_mesh_component_system.cpp
)
set(HDRS
    include/sigmafive/application.hpp
    include/sigmafive/context.hpp
    include/sigmafive/object.hpp

    include/sigmafive/type_info.hpp
    include/sigmafive/compile_time_hash.hpp
    include/sigmafive/serialization/unique_ptr.hpp

    include/sigmafive/math/mat2.hpp
    include/sigmafive/math/mat3.hpp
    include/sigmafive/math/mat4.hpp
    include/sigmafive/math/vec2.hpp
    include/sigmafive/math/vec3.hpp
    include/sigmafive/math/vec4.hpp
    include/sigmafive/math/utils.hpp
    include/sigmafive/math/quaternion.hpp

    include/sigmafive/system/window.hpp
    include/sigmafive/system/resource.hpp
    include/sigmafive/system/resource_manager.hpp

    include/sigmafive/graphics/graphics_system.hpp
    include/sigmafive/graphics/static_mesh.hpp

    generated/gl_core_4_0.hpp
    include/sigmafive/graphics/opengl/buffer.hpp
    include/sigmafive/graphics/opengl/frame_buffer.hpp
    include/sigmafive/graphics/opengl/index_buffer.hpp
    include/sigmafive/graphics/opengl/material.hpp
    include/sigmafive/graphics/opengl/ogl_graphics_system.hpp
    include/sigmafive/graphics/opengl/program.hpp
    include/sigmafive/graphics/opengl/scene_renderer.hpp
    include/sigmafive/graphics/opengl/shader.hpp
    include/sigmafive/graphics/opengl/static_mesh.hpp
    include/sigmafive/graphics/opengl/static_mesh_manager.hpp
    include/sigmafive/graphics/opengl/texture_2d.hpp
    include/sigmafive/graphics/opengl/util.hpp
    include/sigmafive/graphics/opengl/vertex_array.hpp
    include/sigmafive/graphics/opengl/vertex_buffer.hpp

    include/sigmafive/game/scene.hpp
    include/sigmafive/game/entity.hpp
    include/sigmafive/game/component.hpp
    include/sigmafive/game/entity_world.hpp
    include/sigmafive/game/entity_manager.hpp
    include/sigmafive/game/component_pool.hpp
    include/sigmafive/game/component_bitset_manager.hpp
    include/sigmafive/game/component_system.hpp
    include/sigmafive/game/component_manager.hpp
    include/sigmafive/game/transform_component.hpp
    include/sigmafive/game/static_mesh_component.hpp
    include/sigmafive/game/component_system_manager.hpp
    include/sigmafive/game/static_mesh_component_system.hpp
)



include_directories(include)
add_library(sigmafive STATIC ${SRCS} ${HDRS})
target_link_libraries(sigmafive ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} SDL2 SDL2main)

add_subdirectory(tests)

#add_executable(sigma-five-sample src/sigmafive/main.cpp )
#target_link_libraries(sigma-five-sample sigma-five)
