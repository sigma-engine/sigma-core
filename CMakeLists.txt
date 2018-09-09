cmake_minimum_required(VERSION 3.5)
project(sigma-core)

add_library(sigma-core STATIC
	src/sigma/frustum.cpp
	src/sigma/game.cpp
	src/sigma/buddy_array_allocator.cpp
	src/sigma/graphics/renderer.cpp
	src/sigma/trackball_controller.cpp
	src/sigma/util/filesystem.cpp
	src/sigma/window.cpp
	src/sigma/graphics/texture.cpp
)

target_include_directories(sigma-core PUBLIC include)
target_compile_definitions(sigma-core PUBLIC -DGLM_ENABLE_EXPERIMENTAL -DGLM_FORCE_CTOR_INIT PRIVATE -DSIGMA_EXPORT)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} Boost::filesystem Boost::serialization Boost::system Threads::Threads glm jsoncpp_lib_static stb::stb)
target_package_resources(sigma-core "${CMAKE_CURRENT_SOURCE_DIR}/resources")
if(COTIRE_CMAKE_MODULE_VERSION)
	cotire(sigma-core)
endif()

add_subdirectory(test)
add_subdirectory(benchmark)
