#include <sigma/graphics/opengl/static_mesh_manager.hpp>

namespace sigma {
namespace opengl {
    static_mesh_manager::static_mesh_manager(resource::cache<graphics::static_mesh>& static_mesh_cache)
        : static_mesh_cache_(static_mesh_cache)
    {
    }

    static_mesh_manager::~static_mesh_manager()
    {
        for (auto& buffer : static_meshes_) {
            glDeleteBuffers(1, &buffer.index_buffer);
            glDeleteBuffers(1, &buffer.vertex_buffer);
            glDeleteVertexArrays(1, &buffer.vertex_array);
        }
    }

    static_mesh_manager::mesh_buffer static_mesh_manager::acquire(const resource::handle<graphics::static_mesh>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= static_meshes_.size())
            static_meshes_.resize(hndl.index + 1, { 0, 0, 0, 0 });

        if (static_meshes_[hndl.index].vertex_array == 0) {
            auto data = static_mesh_cache_.acquire(hndl);

            glGenVertexArrays(1, &static_meshes_[hndl.index].vertex_array);
            glBindVertexArray(static_meshes_[hndl.index].vertex_array);
            glGenBuffers(1, &static_meshes_[hndl.index].vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, static_meshes_[hndl.index].vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh::vertex) * data->vertices.size(), data->vertices.data(), GL_STATIC_DRAW);

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

            static_meshes_[hndl.index].index_count = GLsizei(3 * data->triangles.size());
            glGenBuffers(1, &static_meshes_[hndl.index].index_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_meshes_[hndl.index].index_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * static_meshes_[hndl.index].index_count, data->triangles.data(), GL_STATIC_DRAW);
        }

        return static_meshes_[hndl.index];
    }
}
}
