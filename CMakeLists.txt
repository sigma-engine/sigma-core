cmake_minimum_required(VERSION 2.8.12)
project(sigma-five-engine)

include(pack-cpp)

add_package(cppbr-math)

if(MSVC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

find_package(Boost REQUIRED COMPONENTS system filesystem)

set(SOURCES
    src/sigmafive/engine.cpp
    src/sigmafive/object.cpp
    src/sigmafive/sha1_hash.cpp

    src/sigmafive/game/component.cpp
    src/sigmafive/game/component_manager.cpp
    src/sigmafive/game/component_registry.cpp
    src/sigmafive/game/component_system.cpp
    src/sigmafive/game/component_system_manager.cpp
    src/sigmafive/game/entity.cpp
    src/sigmafive/game/entity_manager.cpp
    src/sigmafive/game/static_mesh_component.cpp
    src/sigmafive/game/static_mesh_component_system.cpp
    src/sigmafive/game/transform_component.cpp

    src/sigmafive/graphics/context.cpp
    src/sigmafive/graphics/context_manager.cpp
    src/sigmafive/graphics/static_mesh.cpp

    src/sigmafive/resource/resource.cpp

    src/sigmafive/system/window.cpp
    src/sigmafive/system/window_manager.cpp
)

set(HEADERS
    include/sigmafive/config.hpp
    include/sigmafive/engine.hpp
    include/sigmafive/factory.hpp
    include/sigmafive/object.hpp
    include/sigmafive/sha1_hash.hpp

    include/sigmafive/game/component.hpp
    include/sigmafive/game/component_manager.hpp
    include/sigmafive/game/component_registry.hpp
    include/sigmafive/game/component_system.hpp
    include/sigmafive/game/component_system_manager.hpp
    include/sigmafive/game/entity.hpp
    include/sigmafive/game/entity_manager.hpp
    include/sigmafive/game/static_mesh_component.hpp
    include/sigmafive/game/static_mesh_component_system.hpp
    include/sigmafive/game/transform_component.hpp

    include/sigmafive/graphics/context.hpp
    include/sigmafive/graphics/context_manager.hpp
    include/sigmafive/graphics/static_mesh.hpp

    include/sigmafive/resource/resource.hpp

    include/sigmafive/serialization/unique_ptr.hpp

    include/sigmafive/system/window.hpp
    include/sigmafive/system/window_manager.hpp
)

add_library(sigma-five-engine SHARED ${SOURCES} ${HEADERS})

target_include_directories(sigma-five-engine PUBLIC include)

target_compile_definitions(sigma-five-engine
PUBLIC
    -D$<$<CONFIG:Debug>:SIGMA_FIVE_DEBUG>
PRIVATE
    -DSIGMA_FIVE_ENGINE_EXPORT
)

if(NOT MSVC)
    target_compile_options(sigma-five-engine PUBLIC -std=c++14 -pedantic -fPIC)
endif()

target_link_libraries(sigma-five-engine cppbr-math ${Boost_LIBRARIES})

if(UNIX)
    target_link_libraries(sigma-five-engine dl)
endif()
