#include <sigma/graphics/opengl/static_mesh_manager.hpp>

namespace sigma {
namespace opengl {
    static_mesh_manager::static_mesh_manager(resource::cache<graphics::static_mesh>& static_mesh_cache)
        : static_mesh_cache_(static_mesh_cache)
    {
    }

    static_mesh_manager::~static_mesh_manager()
    {
        for (auto& batch : batches_) {
            glDeleteBuffers(1, &batch.index_buffer);
            glDeleteBuffers(1, &batch.vertex_buffer);
            glDeleteVertexArrays(1, &batch.vertex_array);
        }
    }

    std::size_t static_mesh_manager::batch_size() const
    {
        return batches_.size();
    }

    void static_mesh_manager::bind_batch(std::size_t batch)
    {
        glBindVertexArray(batches_[batch].vertex_array);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batches_[batch].index_buffer);
    }

    std::pair<graphics::static_mesh*, static_mesh_manager::mesh_buffer> static_mesh_manager::acquire(const resource::handle<graphics::static_mesh>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= static_meshes_.size())
            static_meshes_.resize(hndl.index + 1, { std::size_t(-1), 0, 0 });

        auto data = static_mesh_cache_.acquire(hndl);
        if (static_meshes_[hndl.index].batch_index == -1) {
            auto vertex_blocks = 1 + ((data->vertices.size() - 1) / VERTEX_BLOCK_SIZE);

            auto index_count = 3 * data->triangles.size();
            auto index_blocks = 1 + ((index_count - 1) / INDEX_BLOCK_SIZE);

            std::size_t vertex_offset = -1;
            std::size_t index_offset = -1;
            std::size_t batch_index = -1;

            for (std::size_t i = 0; i < batches_.size(); ++i) {
                vertex_offset = batches_[i].vertex_allocator.allocate(vertex_blocks);
                if (vertex_offset != -1) {
                    index_offset = batches_[i].index_allocator.allocate(index_blocks);
                    if (index_offset != -1) {
                        batch_index = i;
                        break;
                    } else {
                        batches_[i].vertex_allocator.deallocate(vertex_offset);
                    }
                }
                vertex_offset = -1;
                index_offset = -1;
            }

            if (batch_index == -1 || vertex_offset == -1 || index_offset == -1) {
                buffer_batch batch;

                glGenVertexArrays(1, &batch.vertex_array);
                glBindVertexArray(batch.vertex_array);

                glGenBuffers(1, &batch.vertex_buffer);
                glBindBuffer(GL_ARRAY_BUFFER, batch.vertex_buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh::vertex) * VERTEX_BLOCK_SIZE * MAX_VERTEX_BLOCKS, nullptr, GL_STATIC_DRAW);

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

                glGenBuffers(1, &batch.index_buffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.index_buffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * INDEX_BLOCK_SIZE * MAX_INDEX_BLOCKS, nullptr, GL_STATIC_DRAW);

                vertex_offset = batch.vertex_allocator.allocate(vertex_blocks);
                index_offset = batch.index_allocator.allocate(index_blocks);
                batch_index = batches_.size();
                batches_.push_back(std::move(batch));
            }

            assert(batch_index != -1);
            assert(vertex_offset != -1);
            assert(index_offset != -1);

            glBindBuffer(GL_ARRAY_BUFFER, batches_[batch_index].vertex_buffer);
            glBufferSubData(GL_ARRAY_BUFFER,
                vertex_offset * VERTEX_BLOCK_SIZE * sizeof(graphics::static_mesh::vertex),
                sizeof(graphics::static_mesh::vertex) * data->vertices.size(),
                data->vertices.data());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batches_[batch_index].index_buffer);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                index_offset * INDEX_BLOCK_SIZE * sizeof(unsigned int),
                3 * sizeof(unsigned int) * data->triangles.size(),
                data->triangles.data());

            static_meshes_[hndl.index].batch_index = batch_index;
            static_meshes_[hndl.index].base_vertex = VERTEX_BLOCK_SIZE * vertex_offset;
            static_meshes_[hndl.index].base_index = INDEX_BLOCK_SIZE * index_offset;
        }

        return { data, static_meshes_[hndl.index] };
    }
}
}
