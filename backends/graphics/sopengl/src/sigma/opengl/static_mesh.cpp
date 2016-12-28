#include <sigma/opengl/static_mesh.hpp>

#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader_technique.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    static_mesh::static_mesh(const std::vector<graphics::static_mesh::vertex>& vertices, const std::vector<graphics::static_mesh::triangle>& triangles, std::shared_ptr<shader_technique> tech)
    {
        GL_CHECK(glGenVertexArrays(1, &vertex_array_));
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glGenBuffers(1, &vertex_buffer_));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh::vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

        GL_CHECK(glEnableVertexAttribArray(0));
        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), 0));

        GL_CHECK(glEnableVertexAttribArray(1));
        GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(sizeof(glm::vec3))));

        GL_CHECK(glEnableVertexAttribArray(2));
        GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3))));

        GL_CHECK(glEnableVertexAttribArray(3));
        GL_CHECK(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(3 * sizeof(glm::vec3))));

        index_count_ = 3 * triangles.size();
        GL_CHECK(glGenBuffers(1, &index_buffer_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count_, triangles.data(), GL_STATIC_DRAW));

        technique_ = tech;
    }

    static_mesh::~static_mesh()
    {
        glDeleteBuffers(1, &index_buffer_);
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteVertexArrays(1, &vertex_array_);
    }

    void static_mesh::render()
    {
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr));
    }

    void static_mesh::render(render_matrices* matrices, texture_unit first_texture_unit)
    {
        technique_->bind(matrices, first_texture_unit);
        render();
    }
}
}
