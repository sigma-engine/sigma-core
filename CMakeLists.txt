cmake_minimum_required(VERSION 3.10)
project(sigma-core)

add_library(sigma-core STATIC
	include/sigma/AABB.hpp
	include/sigma/buddy_array_allocator.hpp
	include/sigma/config.hpp
	include/sigma/context.hpp
	include/sigma/frustum.hpp
	include/sigma/game.hpp
	include/sigma/graphics/buffer.hpp
	include/sigma/graphics/cubemap.hpp
	include/sigma/graphics/directional_light.hpp
	include/sigma/graphics/material.hpp
	include/sigma/graphics/pipeline.hpp
	include/sigma/graphics/point_light.hpp
	include/sigma/graphics/post_process_effect.hpp
	include/sigma/graphics/renderer.hpp
	include/sigma/graphics/shader.hpp
	include/sigma/graphics/shadow_block.hpp
	include/sigma/graphics/spot_light.hpp
	include/sigma/graphics/standard_block.hpp
	include/sigma/graphics/static_mesh_instance.hpp
	include/sigma/graphics/static_mesh.hpp
	include/sigma/graphics/technique.hpp
	include/sigma/graphics/texture.hpp
	include/sigma/handle.hpp
	include/sigma/resource/cache.hpp
	include/sigma/resource/resource.hpp
	include/sigma/trackball_controller.hpp
	include/sigma/transform.hpp
	include/sigma/util/filesystem.hpp
	include/sigma/util/glm_serialize.hpp
	include/sigma/util/hash.hpp
	include/sigma/util/numeric.hpp
	include/sigma/util/std140_conversion.hpp
	include/sigma/util/string.hpp
	include/sigma/util/type_sequence.hpp
	include/sigma/util/variadic.hpp
	include/sigma/window.hpp
	src/sigma/buddy_array_allocator.cpp
	src/sigma/context.cpp
	src/sigma/frustum.cpp
	src/sigma/game.cpp
	src/sigma/graphics/buffer.cpp
	src/sigma/graphics/material.cpp
	src/sigma/graphics/pipeline.cpp
	src/sigma/graphics/renderer.cpp
	src/sigma/graphics/shader.cpp
	src/sigma/graphics/static_mesh.cpp
	src/sigma/graphics/texture.cpp
	src/sigma/resource/cache.cpp
	src/sigma/resource/resource.cpp
	src/sigma/trackball_controller.cpp
	src/sigma/util/filesystem.cpp
	src/sigma/window.cpp

)

target_include_directories(sigma-core PUBLIC include)
target_compile_definitions(sigma-core PUBLIC -DCEREAL_FUTURE_EXPERIMENTAL -DGLM_ENABLE_EXPERIMENTAL -DGLM_FORCE_CTOR_INIT PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} Threads::Threads cereal::cereal glm)

if(COTIRE_CMAKE_MODULE_VERSION)
	cotire(sigma-core)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_link_libraries(sigma-core PUBLIC c++experimental)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_link_libraries(sigma-core PUBLIC stdc++fs)
endif()

add_subdirectory(test)
add_subdirectory(benchmark)
