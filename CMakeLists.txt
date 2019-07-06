cmake_minimum_required(VERSION 3.7)
project(sigma-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glad REQUIRED)
find_package(Vulkan REQUIRED)

include(cmake/GLSL.cmake)

add_library(sigma-core STATIC
	include/sigma/Rect.hpp
	include/sigma/Event.hpp
	include/sigma/EventEmitter.hpp
	include/sigma/EventListener.hpp

	include/sigma/DataTypes.hpp
	src/sigma/DataTypes.cpp
	
	include/sigma/DeviceManager.hpp
	src/sigma/DeviceManager.cpp

	include/sigma/Device.hpp
	src/sigma/Device.cpp

	include/sigma/CommandBuffer.hpp
	src/sigma/CommandBuffer.cpp

	include/sigma/Framebuffer.hpp
	src/sigma/Framebuffer.cpp

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

	include/sigma/OpenGL/FramebufferGL.hpp
	src/sigma/OpenGL/FramebufferGL.cpp

	include/sigma/OpenGL/SurfaceGL.hpp
	src/sigma/OpenGL/SurfaceGL.cpp

	include/sigma/OpenGL/DataTypesGL.hpp
	src/sigma/OpenGL/DataTypesGL.cpp

	include/sigma/OpenGL/ShaderGL.hpp
	src/sigma/OpenGL/ShaderGL.cpp

	include/sigma/OpenGL/RenderPassGL.hpp
	src/sigma/OpenGL/RenderPassGL.cpp

	include/sigma/OpenGL/DescriptorSetGL.hpp

	include/sigma/OpenGL/PipelineGL.hpp
	src/sigma/OpenGL/PipelineGL.cpp

	include/sigma/OpenGL/VertexBufferGL.hpp
	src/sigma/OpenGL/VertexBufferGL.cpp

	include/sigma/OpenGL/IndexBufferGL.hpp
	src/sigma/OpenGL/IndexBufferGL.cpp

	include/sigma/Vulkan/DeviceManagerVK.hpp
	src/sigma/Vulkan/DeviceManagerVK.cpp

	include/sigma/Vulkan/DeviceVK.hpp
	src/sigma/Vulkan/DeviceVK.cpp

	include/sigma/Vulkan/CommandBufferVK.hpp
	src/sigma/Vulkan/CommandBufferVK.cpp

	include/sigma/Vulkan/FramebufferVK.hpp
	src/sigma/Vulkan/FramebufferVK.cpp

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

	src/sigma/Vulkan/ExtensionsVK.cpp

	include/sigma/Vulkan/UtilVK.hpp
	src/sigma/Vulkan/UtilVK.cpp
)

target_include_directories(sigma-core
PUBLIC
	include
)

target_link_libraries(sigma-core
PUBLIC
    ${CMAKE_DL_LIBS}
    spdlog::spdlog
    SDL2::SDL2-static
    glad::glad
    Vulkan::Vulkan
    glm
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
)

add_executable(simple-game
	shaders/simple.vert.glsl
	shaders/simple.frag.glsl

	src/SimpleRenderer.cpp
	include/SimpleRenderer.hpp
	src/main.cpp
)
compile_glsl_sources(simple-game)

target_link_libraries(simple-game
PUBLIC
	sigma-core
)
