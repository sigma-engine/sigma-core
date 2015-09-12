project(OpenGL)

find_package(OpenGL REQUIRED)

set(SOURCES
    src/buffer.cpp
    src/context.cpp
    src/frame_buffer.cpp
    src/gl_core_4_0.cpp
    src/index_buffer.cpp
    src/material.cpp
    src/program.cpp
    src/shader.cpp
    src/static_mesh.cpp
    src/static_mesh_manager.cpp
    src/texture_2d.cpp
    src/vertex_array.cpp
    src/vertex_buffer.cpp
)
set(HEADERS
    include/buffer.hpp
    include/context.hpp
    include/frame_buffer.hpp
    include/gl_core_4_0.hpp
    include/index_buffer.hpp
    include/material.hpp
    include/program.hpp
    include/shader.hpp
    include/static_mesh.hpp
    include/static_mesh_manager.hpp
    include/texture_2d.hpp
    include/util.hpp
    include/vertex_array.hpp
    include/vertex_buffer.hpp
)

add_plugin(OpenGL ${SOURCES} ${HEADERS})
target_include_directories(OpenGL 
PUBLIC
    include
PRIVATE
    ${OPENGL_INCLUDE_DIRS}
)
target_link_libraries(OpenGL engine ${OPENGL_LIBRARIES})
