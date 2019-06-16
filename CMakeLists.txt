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

	include/sigma/SDLWindow.hpp
	src/sigma/SDLWindow.cpp

	include/sigma/GLContext.hpp
	src/sigma/GLContext.cpp

	include/sigma/GLDataTypes.hpp
	src/sigma/GLDataTypes.cpp

	include/sigma/GLShader.hpp
	src/sigma/GLShader.cpp

	include/sigma/GLProgram.hpp
	src/sigma/GLProgram.cpp

	include/sigma/GLVertexBuffer.hpp
	src/sigma/GLVertexBuffer.cpp

	include/sigma/GLIndexBuffer.hpp
	src/sigma/GLIndexBuffer.cpp
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
