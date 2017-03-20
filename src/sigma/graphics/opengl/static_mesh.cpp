#include <sigma/graphics/opengl/static_mesh.hpp>

#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader_technique.hpp>
#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    static_mesh::static_mesh(const graphics::static_mesh_data& data)
        : graphics::static_mesh(data)
    {
        GL_CHECK(glGenVertexArrays(1, &vertex_array_));
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glGenBuffers(1, &vertex_buffer_));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh_data::vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW));

        GL_CHECK(glEnableVertexAttribArray(0));
        GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh_data::vertex), 0));

        GL_CHECK(glEnableVertexAttribArray(1));
        GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh_data::vertex), reinterpret_cast<const void*>(sizeof(glm::vec3))));

        GL_CHECK(glEnableVertexAttribArray(2));
        GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh_data::vertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3))));

        GL_CHECK(glEnableVertexAttribArray(3));
        GL_CHECK(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh_data::vertex), reinterpret_cast<const void*>(3 * sizeof(glm::vec3))));

        index_count_ = GLsizei(3 * data.triangles.size());
        GL_CHECK(glGenBuffers(1, &index_buffer_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * index_count_, data.triangles.data(), GL_STATIC_DRAW));
    }

    static_mesh::~static_mesh()
    {
        glDeleteBuffers(1, &index_buffer_);
        glDeleteBuffers(1, &vertex_buffer_);
        glDeleteVertexArrays(1, &vertex_array_);
    }

    void static_mesh::render() const
    {
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr));
    }

    void static_mesh::render(unsigned int material_slot) const
    {
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glDrawElements(GL_TRIANGLES, GLsizei(3 * material_slots_[material_slot].second), GL_UNSIGNED_INT, reinterpret_cast<const void*>(sizeof(graphics::static_mesh_data::triangle) * material_slots_[material_slot].first)));
    }

    void static_mesh::render_all() const
    {
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
        GL_CHECK(glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0));
    }

    static_mesh_manager::static_mesh_manager(boost::filesystem::path cache_directory, opengl::material_manager& materials)
        : graphics::static_mesh_manager(cache_directory)
        , materials_(materials)
    {
    }

    std::unique_ptr<graphics::static_mesh> static_mesh_manager::create(graphics::static_mesh_data data)
    {
        auto mesh = std::make_unique<opengl::static_mesh>(std::move(data));
        for (unsigned int i = 0; i < mesh->material_count(); ++i)
            mesh->material(i).set_manager(&materials_);
        return std::move(mesh);
    }
}
}
