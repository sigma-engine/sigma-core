cmake_minimum_required(VERSION 2.8.12)
project(sigma-five)

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
    src/system/window.cpp
    src/system/resource.cpp
    src/system/resource_manager.cpp

    src/graphics/static_mesh.cpp

    generated/gl_core_4_0.cpp
    src/graphics/opengl/buffer.cpp
    src/graphics/opengl/shader.cpp
    src/graphics/opengl/program.cpp
    src/graphics/opengl/texture_2d.cpp
    src/graphics/opengl/vertex_array.cpp
    src/graphics/opengl/index_buffer.cpp
    src/graphics/opengl/vertex_buffer.cpp

    src/graphics/opengl/static_mesh.cpp
    src/graphics/opengl/static_mesh_manager.cpp
    src/graphics/opengl/scene_renderer.cpp

    src/game/scene.cpp
    src/game/entity.cpp
    src/game/component.cpp
    src/game/entity_manager.cpp
    src/game/component_pool.cpp
    src/game/component_system.cpp
    src/game/transform_component.cpp
    src/game/static_mesh_component.cpp
    src/game/static_mesh_component_system.cpp

    #src/main.cpp
)
set(HDRS
    include/type_info.hpp

    include/math/mat2.hpp
    include/math/mat3.hpp
    include/math/mat4.hpp
    include/math/vec2.hpp
    include/math/vec3.hpp
    include/math/vec4.hpp
    include/math/utils.hpp
    include/math/quaternion.hpp

    include/graphics/static_mesh.hpp

    generated/gl_core_4_0.hpp
    include/graphics/opengl/util.hpp
    include/graphics/opengl/buffer.hpp
    include/graphics/opengl/shader.hpp
    include/graphics/opengl/program.hpp
    include/graphics/opengl/texture_2d.hpp
    include/graphics/opengl/vertex_array.hpp
    include/graphics/opengl/index_buffer.hpp
    include/graphics/opengl/vertex_buffer.hpp

    include/graphics/opengl/static_mesh.hpp
    include/graphics/opengl/static_mesh_manager.hpp
    include/graphics/opengl/scene_renderer.hpp

    include/system/window.hpp
    include/system/resource.hpp
    include/system/resource_manager.hpp

    include/game/scene.hpp
    include/game/entity.hpp
    include/game/component.hpp
    include/game/entity_manager.hpp
    include/game/component_pool.hpp
    include/game/component_system.hpp
    include/game/transform_component.hpp
    include/game/static_mesh_component.hpp
    include/game/static_mesh_component_system.hpp
)



include_directories(include)
add_library(sigma-five STATIC ${SRCS} ${HDRS})
target_link_libraries(sigma-five ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} SDL2 SDL2main)

add_subdirectory(tests)

add_executable(sigma-five-sample src/main.cpp )
target_link_libraries(sigma-five-sample sigma-five)