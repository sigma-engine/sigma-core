cmake_minimum_required(VERSION 3.2)
project(sigma-engine)

include(conanbuildinfo.cmake)
conan_basic_setup()

include(sigma_setup.cmake)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

add_subdirectory(tools/screflect)
add_subdirectory(tools/sctexture)
add_subdirectory(tools/scshader)
add_subdirectory(tools/scmaterial)
add_subdirectory(tools/scmodel)
add_subdirectory(backends/graphics/sopengl)
add_subdirectory(tools/seditor)

set(SOURCES
    src/sigma/entity.cpp

    src/sigma/game.cpp
    src/sigma/entity_manager.cpp

    src/sigma/util/filesystem.cpp

    src/sigma/resource/identifier.cpp

    src/sigma/graphics/context.cpp
    src/sigma/graphics/texture.cpp
    src/sigma/graphics/shader.cpp
    src/sigma/graphics/material.cpp
    src/sigma/graphics/static_mesh.cpp
    src/sigma/graphics/static_mesh_instance.cpp
)

set(HEADERS
    include/sigma/config.hpp

    include/sigma/reflect/reflect.hpp

    include/sigma/game.hpp
    include/sigma/entity.hpp
    include/sigma/component.hpp
    include/sigma/entity_manager.hpp

    include/sigma/transform.hpp

    include/sigma/util/compile_time_hash.hpp
    include/sigma/util/glm_serialize.hpp
    include/sigma/util/filesystem.hpp

    include/sigma/resource/identifier.hpp
    include/sigma/resource/basic_resource_cache.hpp

    include/sigma/graphics/context.hpp
    include/sigma/graphics/texture.hpp
    include/sigma/graphics/shader.hpp
    include/sigma/graphics/material.hpp
    include/sigma/graphics/directional_light.hpp
    include/sigma/graphics/point_light.hpp
    include/sigma/graphics/spot_light.hpp
    include/sigma/graphics/static_mesh.hpp
    include/sigma/graphics/static_mesh_instance.hpp
)

add_library(sigma-engine SHARED ${SOURCES} ${HEADERS})
target_include_directories(sigma-engine PUBLIC include)
target_compile_options(sigma-engine PRIVATE -DSIGMA_ENGINE_EXPORT)
target_link_libraries(sigma-engine PUBLIC pthread ${CONAN_LIBS})

#add_subdirectory(tests)
