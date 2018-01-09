#include <sigma/graphics/opengl/static_mesh.hpp>

#include <sigma/graphics/opengl/material.hpp>
#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/static_mesh.hpp>

#include <glad/glad.h>

#include <boost/filesystem/path.hpp>

namespace sigma {
namespace opengl {
    static_mesh::static_mesh(material_manager& material_mgr, const graphics::static_mesh& data)
        : data(data)
    {
        glGenVertexArrays(1, &vertex_array_);
        glBindVertexArray(vertex_array_);
        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh::vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), 0);

        // normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(sizeof(glm::vec3)));

        // bitangent
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3)));

        // tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(3 * sizeof(glm::vec3)));

        // texcoord
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(4 * sizeof(glm::vec3)));

        index_count_ = GLsizei(3 * data.triangles.size());
        glGenBuffers(1, &index_buffer_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count_, data.triangles.data(), GL_STATIC_DRAW);
    }

    static_mesh::~static_mesh()
    {
        glDeleteBuffers(1, &index_buffer_);
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteVertexArrays(1, &vertex_array_);
    }

    void static_mesh::render(unsigned int material_slot) const
    {
        glBindVertexArray(vertex_array_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
        glDrawElements(GL_TRIANGLES, GLsizei(3 * data.material_slots[material_slot].second), GL_UNSIGNED_INT, reinterpret_cast<const void*>(sizeof(graphics::static_mesh::triangle) * data.material_slots[material_slot].first));
    }

    void static_mesh::render_all() const
    {
        glBindVertexArray(vertex_array_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0);
    }
}
}
