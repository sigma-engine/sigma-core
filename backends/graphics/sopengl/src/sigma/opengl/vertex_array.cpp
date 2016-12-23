#include <sigma/opengl/vertex_array.hpp>

#include <sigma/opengl/util.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    vertex_array::vertex_array()
        : gl_object_(0)
    {
        GL_CHECK(glGenVertexArrays(1, &gl_object_));
    }

    vertex_array::~vertex_array()
    {
        glDeleteVertexArrays(1, &gl_object_); // TODO GL_CHECK_ERROR;don't throw in the destructor
    }

    void vertex_array::bind(const vertex_buffer_base& buffer, unsigned int index,
        unsigned int number_of_components,
        data_types component_type, bool normalize,
        const void* offset) const
    {
        // GL_CHECK_ERROR(glBindVertexArray(gl_object_));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buffer));

        GL_CHECK(glEnableVertexAttribArray(index)); // TODO does this change gloabl state
        GL_CHECK(glVertexAttribPointer(index, number_of_components, component_type, static_cast<GLboolean>(normalize), static_cast<GLsizei>(buffer.stride()), offset));
    }

    void vertex_array::draw(primitive_type mode, std::size_t first,
        std::size_t count) const
    {
        // TODO preserve opengl vertex array state??? or use direct state if is is
        // available.
        GL_CHECK(glBindVertexArray(gl_object_));
        GL_CHECK(glDrawArrays(static_cast<GLenum>(mode), first, count));
    }

    void vertex_array::draw(primitive_type mode, GLuint index_buffer,
        std::size_t first, std::size_t count) const
    {
        // GL_CHECK_ERROR(glBindVertexArray(gl_object_));
        // GL_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
        GL_CHECK(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)first));
    }
}
}
