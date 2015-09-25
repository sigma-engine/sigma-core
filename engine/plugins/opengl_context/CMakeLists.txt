project(opengl_context)

find_package(OpenGL REQUIRED)

set(SOURCES
	src/g_buffer.cpp
	src/material.cpp
	src/static_mesh.cpp
	src/static_mesh_manager.cpp
	src/context.cpp)

set(HEADERS
	include/g_buffer.hpp
	include/material.hpp
	include/static_mesh.hpp
	include/static_mesh_manager.hpp
	include/context.hpp)

add_plugin(opengl_context ${SOURCES} ${HEADERS})
target_include_directories(opengl_context
	PUBLIC
	include
	PRIVATE
	${OPENGL_INCLUDE_DIRS})
target_link_libraries(opengl_context cppbr-opengl engine ${OPENGL_LIBRARIES})
