cmake_minimum_required(VERSION 3.2)
project(sigma-five-engine)

set(SOURCES
    src/sigmafive/entity.cpp

    src/sigmafive/game.cpp
    src/sigmafive/entity_manager.cpp

    src/sigmafive/graphics/context.cpp
    src/sigmafive/graphics/shader.cpp
    src/sigmafive/graphics/material.cpp
    src/sigmafive/graphics/static_mesh.cpp
    src/sigmafive/graphics/static_mesh_instance.cpp
)

set(HEADERS
    include/sigmafive/config.hpp

    include/sigmafive/game.hpp
    include/sigmafive/entity.hpp
    include/sigmafive/component.hpp
    include/sigmafive/entity_manager.hpp

    include/sigmafive/transform.hpp

    include/sigmafive/util/compile_time_hash.hpp
    include/sigmafive/util/glm_serialize.hpp

    include/sigmafive/resource/identifier.hpp
    include/sigmafive/resource/basic_resource_cache.hpp

    include/sigmafive/graphics/context.hpp
    include/sigmafive/graphics/texture.hpp
    include/sigmafive/graphics/shader.hpp
    include/sigmafive/graphics/material.hpp
    include/sigmafive/graphics/directional_light.hpp
    include/sigmafive/graphics/point_light.hpp
    include/sigmafive/graphics/spot_light.hpp
    include/sigmafive/graphics/static_mesh.hpp
    include/sigmafive/graphics/static_mesh_instance.hpp
)

add_library(sigma-five-engine SHARED ${SOURCES} ${HEADERS})

target_include_directories(sigma-five-engine PUBLIC include)

target_compile_options(sigma-five-engine PRIVATE -DSIGMA_FIVE_ENGINE_EXPORT)

target_link_libraries(sigma-five-engine glm ${Boost_LIBRARIES})

if(NOT MSVC)
    target_compile_options(sigma-five-engine PUBLIC -std=c++14 -pedantic -fvisibility=hidden)
endif()

add_subdirectory(tests)
