#include <sigma/opengl/static_mesh.hpp>

#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader_technique.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    static_mesh::static_mesh(graphics::static_mesh_data data)
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

		index_count_ = 3 * data.triangles.size();
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

    void static_mesh::render()
    {
        GL_CHECK(glBindVertexArray(vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_));
		GL_CHECK(glDrawRangeElements(GL_TRIANGLES,0, index_count_, index_count_, GL_UNSIGNED_INT,nullptr));
    }

    static_mesh_manager::static_mesh_manager(boost::filesystem::path cache_directory, opengl::material_manager& materials)
        : graphics::static_mesh_manager(cache_directory)
        , materials_(materials)
    {
    }

    std::unique_ptr<graphics::static_mesh> static_mesh_manager::load(graphics::static_mesh_data data, boost::archive::binary_iarchive& ia)
    {
        auto mat = materials_.get(data.materials.begin()->first); // TODO support more than one material
        auto mesh = std::make_unique<opengl::static_mesh>(std::move(data));
        mesh->set_material(mat);
        return std::move(mesh);
    }
}
}
