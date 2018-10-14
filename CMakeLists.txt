cmake_minimum_required(VERSION 3.10)
project(sigma-core)

add_library(sigma-core STATIC
		src/sigma/context.cpp
		src/sigma/graphics/buffer.cpp
		src/sigma/graphics/material.cpp
		src/sigma/graphics/shader.cpp
		src/sigma/resource/cache.cpp
		src/sigma/resource/resource.cpp
		src/sigma/util/glm_json.cpp
        src/sigma/buddy_array_allocator.cpp
        src/sigma/frustum.cpp
        src/sigma/game.cpp
        src/sigma/graphics/pipeline.cpp
        src/sigma/graphics/renderer.cpp
        src/sigma/graphics/texture.cpp
        src/sigma/trackball_controller.cpp
        src/sigma/util/filesystem.cpp
        src/sigma/window.cpp
)

target_include_directories(sigma-core PUBLIC include)
target_compile_definitions(sigma-core PUBLIC -DGLM_ENABLE_EXPERIMENTAL -DGLM_FORCE_CTOR_INIT PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} cereal::cereal Boost::filesystem Boost::system Threads::Threads glm nlohmann_json::nlohmann_json)

if(COTIRE_CMAKE_MODULE_VERSION)
	cotire(sigma-core)
endif()

add_subdirectory(test)
add_subdirectory(benchmark)
