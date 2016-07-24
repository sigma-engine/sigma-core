cmake_minimum_required(VERSION 3.2)
project(engine)

add_subdirectory(tools/reflector)
add_subdirectory(tools/texture-compiler)
add_subdirectory(tools/shader-compiler)
add_subdirectory(tools/material-compiler)
add_subdirectory(tools/model-compiler)
add_subdirectory(tools/editor)

set(SOURCES
    src/sigmafive/entity.cpp

    src/sigmafive/game.cpp
    src/sigmafive/entity_manager.cpp

    src/sigmafive/util/filesystem.cpp

    src/sigmafive/resource/identifier.cpp

    src/sigmafive/graphics/context.cpp
    src/sigmafive/graphics/texture.cpp
    src/sigmafive/graphics/shader.cpp
    src/sigmafive/graphics/material.cpp
    src/sigmafive/graphics/static_mesh.cpp
    src/sigmafive/graphics/static_mesh_instance.cpp
)

set(HEADERS
    include/sigmafive/config.hpp

    include/sigmafive/reflect/reflect.hpp

    include/sigmafive/game.hpp
    include/sigmafive/entity.hpp
    include/sigmafive/component.hpp
    include/sigmafive/entity_manager.hpp

    include/sigmafive/transform.hpp

    include/sigmafive/util/compile_time_hash.hpp
    include/sigmafive/util/glm_serialize.hpp
    include/sigmafive/util/filesystem.hpp

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

add_library(engine STATIC ${SOURCES} ${HEADERS})
add_dependencies(engine reflector)

target_include_directories(engine PUBLIC include)

target_compile_options(engine PRIVATE -DSIGMA_FIVE_ENGINE_EXPORT)

target_link_libraries(engine pthread ${CONAN_LIBS})

#add_subdirectory(tests)
