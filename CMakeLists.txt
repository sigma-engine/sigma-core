cmake_minimum_required(VERSION 3.7)
project(sigma-engine)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
	set(SIGMA_VULKAN_SUPPORT OFF)
else()
	set(SIGMA_VULKAN_SUPPORT ON)
endif()

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(stb REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

if (${SIGMA_VULKAN_SUPPORT})
	find_package(Vulkan REQUIRED)
	find_package(unofficial-vulkan-memory-allocator CONFIG REQUIRED)
endif()

include(cmake/GLSL.cmake)
include(cmake/Texture.cmake)

add_library(sigma-core STATIC
	include/sigma/Algorithm.hpp
	include/sigma/FlagSet.hpp
	include/sigma/Rect.hpp
	include/sigma/Event.hpp
	include/sigma/EventEmitter.hpp
	include/sigma/EventListener.hpp
	include/sigma/CameraComponent.hpp

	include/sigma/Context.hpp
	src/sigma/Context.cpp

	include/sigma/Resource/AssetManager.hpp
	src/sigma/Resource/AssetManager.cpp

	include/sigma/Resource/AssetCache.hpp
	src/sigma/Resource/AssetCache.cpp

	include/sigma/Resource/AssetLoader.hpp
	src/sigma/Resource/AssetLoader.cpp

	include/sigma/Resource/StbImageLoader.hpp
	src/sigma/Resource/StbImageLoader.cpp

	include/sigma/DataTypes.hpp
	src/sigma/DataTypes.cpp
	
	include/sigma/DeviceManager.hpp
	src/sigma/DeviceManager.cpp

	include/sigma/Device.hpp
	src/sigma/Device.cpp

	include/sigma/CommandBuffer.hpp
	src/sigma/CommandBuffer.cpp

	include/sigma/FrameBuffer.hpp
	src/sigma/FrameBuffer.cpp

	include/sigma/Surface.hpp
	src/sigma/Surface.cpp

	include/sigma/Shader.hpp
	src/sigma/Shader.cpp

	include/sigma/RenderPass.hpp
	src/sigma/RenderPass.cpp

	include/sigma/VertexLayout.hpp
	src/sigma/VertexLayout.cpp

	include/sigma/Pipeline.hpp

	include/sigma/VertexBuffer.hpp

	include/sigma/IndexBuffer.hpp

	include/sigma/DescriptorSet.hpp

	include/sigma/UniformBuffer.hpp

	include/sigma/Texture.hpp
	src/sigma/Texture.cpp

	include/sigma/Sampler.hpp

	include/sigma/Log.hpp
	
	include/sigma/Engine.hpp
	src/sigma/Engine.cpp

	include/sigma/Window.hpp
	src/sigma/Window.cpp

	include/sigma/SDL/WindowSDL.hpp
	src/sigma/SDL/WindowSDL.cpp

	include/sigma/OpenGL/DeviceManagerGL.hpp
	src/sigma/OpenGL/DeviceManagerGL.cpp

	include/sigma/OpenGL/DeviceGL.hpp
	src/sigma/OpenGL/DeviceGL.cpp

	include/sigma/OpenGL/CommandBufferGL.hpp
	src/sigma/OpenGL/CommandBufferGL.cpp

	include/sigma/OpenGL/FrameBufferGL.hpp
	src/sigma/OpenGL/FrameBufferGL.cpp

	include/sigma/OpenGL/SurfaceGL.hpp
	src/sigma/OpenGL/SurfaceGL.cpp

	include/sigma/OpenGL/UtilGL.hpp
	src/sigma/OpenGL/UtilGL.cpp

	include/sigma/OpenGL/ShaderGL.hpp
	src/sigma/OpenGL/ShaderGL.cpp

	include/sigma/OpenGL/RenderPassGL.hpp
	src/sigma/OpenGL/RenderPassGL.cpp

	include/sigma/OpenGL/DescriptorSetGL.hpp
	src/sigma/OpenGL/DescriptorSetGL.cpp

	include/sigma/OpenGL/UniformBufferGL.hpp
	src/sigma/OpenGL/UniformBufferGL.cpp
	
	include/sigma/OpenGL/TextureGL.hpp
	src/sigma/OpenGL/TextureGL.cpp

	include/sigma/OpenGL/SamplerGL.hpp
	src/sigma/OpenGL/SamplerGL.cpp

	include/sigma/OpenGL/PipelineGL.hpp
	src/sigma/OpenGL/PipelineGL.cpp

	include/sigma/OpenGL/VertexBufferGL.hpp
	src/sigma/OpenGL/VertexBufferGL.cpp

	include/sigma/OpenGL/IndexBufferGL.hpp
	src/sigma/OpenGL/IndexBufferGL.cpp
)

if (${SIGMA_VULKAN_SUPPORT})
	target_sources(sigma-core PRIVATE 
		include/sigma/Vulkan/DeviceManagerVK.hpp
		src/sigma/Vulkan/DeviceManagerVK.cpp
		include/sigma/Vulkan/DeviceVK.hpp
		src/sigma/Vulkan/DeviceVK.cpp
		include/sigma/Vulkan/CommandBufferVK.hpp
		src/sigma/Vulkan/CommandBufferVK.cpp
		include/sigma/Vulkan/FrameBufferVK.hpp
		src/sigma/Vulkan/FrameBufferVK.cpp
		include/sigma/Vulkan/SurfaceVK.hpp
		src/sigma/Vulkan/SurfaceVK.cpp
		include/sigma/Vulkan/ShaderVK.hpp
		src/sigma/Vulkan/ShaderVK.cpp
		include/sigma/Vulkan/RenderPassVK.hpp
		src/sigma/Vulkan/RenderPassVK.cpp
		include/sigma/Vulkan/PipelineVK.hpp
		src/sigma/Vulkan/PipelineVK.cpp
		include/sigma/Vulkan/VertexBufferVK.hpp
		src/sigma/Vulkan/VertexBufferVK.cpp
		include/sigma/Vulkan/IndexBufferVK.hpp
		src/sigma/Vulkan/IndexBufferVK.cpp
		include/sigma/Vulkan/DescriptorSetVK.hpp
		src/sigma/Vulkan/DescriptorSetVK.cpp
		include/sigma/Vulkan/UniformBufferVK.hpp
		src/sigma/Vulkan/UniformBufferVK.cpp
		include/sigma/Vulkan/TextureVK.hpp
		src/sigma/Vulkan/TextureVK.cpp
		include/sigma/Vulkan/SamplerVK.hpp
		src/sigma/Vulkan/SamplerVK.cpp
		src/sigma/Vulkan/ExtensionsVK.cpp
		include/sigma/Vulkan/UtilVK.hpp
		src/sigma/Vulkan/UtilVK.cpp
	)

	target_compile_definitions(sigma-core
	PUBLIC
		SIGMA_VULKAN_SUPPORT
	)

	target_link_libraries(sigma-core
	PRIVATE
		Vulkan::Vulkan
		unofficial::vulkan-memory-allocator::vulkan-memory-allocator
	)
endif()

target_include_directories(sigma-core
PUBLIC
	include
PRIVATE
	${Stb_INCLUDE_DIR}
)

target_link_libraries(sigma-core
PUBLIC
	${CMAKE_DL_LIBS}
	spdlog::spdlog
	SDL2::SDL2
	SDL2::SDL2main
	glad::glad
	glm::glm
)

if(NOT WIN32)
	target_link_libraries(sigma-core
	PUBLIC
		stdc++fs
	)
endif()

target_compile_definitions(sigma-core
PUBLIC
	-DGLM_ENABLE_EXPERIMENTAL
	-DGLM_FORCE_CTOR_INIT
	-DGLM_FORCE_RADIANS
)

add_executable(simple-game
	shaders/simple.vert.glsl
	shaders/simple.frag.glsl
	textures/TestImage.jpg

	src/SimpleRenderer.cpp
	include/SimpleRenderer.hpp
	
	src/SimpleCameraController.cpp
	include/SimpleCameraController.hpp

	src/SimpleGame.cpp
	include/SimpleGame.hpp
	src/main.cpp
)
compile_glsl_sources(simple-game)
copy_texture_sources(simple-game)

target_link_libraries(simple-game
PUBLIC
	sigma-core
)
