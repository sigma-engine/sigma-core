cmake_minimum_required(VERSION 3.7)
project(sigma-engine)

find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glad REQUIRED)

add_library(sigma-core
	include/sigma/Event.hpp
	include/sigma/EventEmitter.hpp
	include/sigma/EventListener.hpp


	include/sigma/Context.hpp
	src/sigma/Context.cpp

	include/sigma/Shader.hpp
	src/sigma/Shader.cpp

	include/sigma/VertexBuffer.hpp
	src/sigma/VertexBuffer.cpp

	include/sigma/IndexBuffer.hpp

	include/sigma/Program.hpp

	include/sigma/Engine.hpp
	src/sigma/Engine.cpp

	include/sigma/Window.hpp
	src/sigma/Window.cpp

	include/sigma/SDL/SDLWindow.hpp
	src/sigma/SDL/SDLWindow.cpp

	include/sigma/OpenGL/GLContext.hpp
	src/sigma/OpenGL/GLContext.cpp

	include/sigma/OpenGL/GLDataTypes.hpp
	src/sigma/OpenGL/GLDataTypes.cpp

	include/sigma/OpenGL/GLShader.hpp
	src/sigma/OpenGL/GLShader.cpp

	include/sigma/OpenGL/GLProgram.hpp
	src/sigma/OpenGL/GLProgram.cpp

	include/sigma/OpenGL/GLVertexBuffer.hpp
	src/sigma/OpenGL/GLVertexBuffer.cpp

	include/sigma/OpenGL/GLIndexBuffer.hpp
	src/sigma/OpenGL/GLIndexBuffer.cpp
)

target_include_directories(sigma-core
PUBLIC
	include
)

target_link_libraries(sigma-core
PUBLIC
    ${CMAKE_DL_LIBS}
    spdlog::spdlog
    SDL2::SDL2
    glad::glad
    glm
    stdc++fs
)

target_compile_definitions(sigma-core
PUBLIC
    -DGLM_ENABLE_EXPERIMENTAL
    -DGLM_FORCE_CTOR_INIT
)

add_executable(simple-game
	src/main.cpp
)

target_link_libraries(simple-game
PUBLIC
	sigma-core
)
