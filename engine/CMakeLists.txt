cmake_minimum_required(VERSION 2.8.12)
project(sigmafive)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
include(plugin_utils)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(SDL2 REQUIRED)

set(SOURCES
    src/sigmafive/object.cpp
    src/sigmafive/engine.cpp

    src/sigmafive/system/window.cpp
    src/sigmafive/system/resource.cpp
    src/sigmafive/system/resource_manager.cpp

    src/sigmafive/graphics/static_mesh.cpp
    src/sigmafive/graphics/context.cpp
    src/sigmafive/graphics/context_manager.cpp

    src/sigmafive/game/entity.cpp
    src/sigmafive/game/component.cpp
    src/sigmafive/game/component_system.cpp
    src/sigmafive/game/component_manager.cpp
    src/sigmafive/game/entity_manager.cpp
    src/sigmafive/game/component_registry.cpp
    src/sigmafive/game/transform_component.cpp
    src/sigmafive/game/static_mesh_component.cpp
    src/sigmafive/game/component_system_manager.cpp
    src/sigmafive/game/static_mesh_component_system.cpp
)
set(HEADERS
	include/sigmafive/config.hpp
    include/sigmafive/object.hpp
    include/sigmafive/factory.hpp
    include/sigmafive/engine.hpp
    include/sigmafive/type_info.hpp

    include/sigmafive/math/mat2.hpp
    include/sigmafive/math/mat3.hpp
    include/sigmafive/math/mat4.hpp
    include/sigmafive/math/vec2.hpp
    include/sigmafive/math/vec3.hpp
    include/sigmafive/math/vec4.hpp
    include/sigmafive/math/utils.hpp
    include/sigmafive/math/quaternion.hpp

    include/sigmafive/serialization/unique_ptr.hpp

    include/sigmafive/system/window.hpp
    include/sigmafive/system/resource.hpp
    include/sigmafive/system/resource_manager.hpp

    include/sigmafive/graphics/static_mesh.hpp
    include/sigmafive/graphics/context.hpp
    include/sigmafive/graphics/context_manager.hpp

    include/sigmafive/game/entity.hpp
    include/sigmafive/game/component.hpp
    include/sigmafive/game/component_system.hpp
    include/sigmafive/game/component_manager.hpp
    include/sigmafive/game/entity_manager.hpp
    include/sigmafive/game/component_registry.hpp
    include/sigmafive/game/transform_component.hpp
    include/sigmafive/game/static_mesh_component.hpp
    include/sigmafive/game/component_system_manager.hpp
    include/sigmafive/game/static_mesh_component_system.hpp
)

link_directories(${Boost_LIBRARY_DIRS})

add_library(sigmafive SHARED ${SOURCES} ${HEADERS})
target_include_directories(sigmafive
PUBLIC
    include
    ${Boost_INCLUDE_DIRS}
PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sigmafive ${Boost_LIBRARIES} ${SDL2_LIBRARIES})

add_subdirectory(plugins)
add_subdirectory(tests)
