cmake_minimum_required(VERSION 3.0)
project(simple-game)
set (CMAKE_CXX_STANDARD 14)
set(SOURCES
    src/simple_game.cpp
)
set(HEADERS
)
add_library(simple-game SHARED ${SOURCES} ${HEADERS})
target_include_directories(simple-game PUBLIC include resources ../backends/graphics/sopengl/resources)
target_link_libraries(simple-game sigma-core)

#add_resource_package(simple-game-resources resources include)
#add_dependencies(simple-game simple-game-resources)

file(GLOB_RECURSE SHADER_SOURCES resources ${SIGMA_ENGINE_SHADER_GLOB})
file(GLOB_RECURSE TEXTURE_SOURCES resources ${SIGMA_ENGINE_TEXTURE_GLOB})
file(GLOB_RECURSE CUBEMAP_SOURCES resources ${SIGMA_ENGINE_CUBEMAP_GLOB})
file(GLOB_RECURSE MATERIAL_SOURCES resources ${SIGMA_ENGINE_MATERIAL_GLOB})
file(GLOB_RECURSE MODEL_SOURCES resources ${SIGMA_ENGINE_MODEL_GLOB})
add_resources(simple-game PACKAGE_ROOT resources ${SHADER_SOURCES} ${TEXTURE_SOURCES} ${CUBEMAP_SOURCES} ${MATERIAL_SOURCES} ${MODEL_SOURCES})
