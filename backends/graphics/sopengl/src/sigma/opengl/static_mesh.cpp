#include <sigma/opengl/static_mesh.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace opengl {
    static_mesh_cache::static_mesh_cache(material_cache& materials)
        : cache_directory_(boost::filesystem::current_path() / ".." / "data")
        , dirty_(static_meshes_.size())
        , materials_(materials)
    {
    }

    static_mesh_cache::~static_mesh_cache()
    {
        for (auto& mesh : static_meshes_) {
            // TODO decrement material?
            glDeleteBuffers(1, &mesh.vertex_array);
            glDeleteBuffers(1, &mesh.index_buffer);
            glDeleteVertexArrays(1, &mesh.vertex_array);
        }
    }

    bool static_mesh_cache::is_cached(resource::identifier mesh) const
    {
        return resource_map_.count(mesh) > 0;
    }

    bool static_mesh_cache::increment_reference(resource::identifier mesh)
    {
        if (is_cached(mesh)) {
            static_meshes_[resource_map_[mesh]].reference_count++;
            return true;
        }

        static_mesh gl_mesh;
        try {
            auto mesh_path = cache_directory_ / std::to_string(mesh.value());
            std::ifstream file{ mesh_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            ia >> gl_mesh;
        } catch (boost::archive::archive_exception& ex) {
            std::cout << "static_mesh: " << mesh << " " << ex.what() << std::endl;
            return false;
        } catch (std::exception& ex) {
            std::cout << "static_mesh: " << mesh << " " << ex.what() << std::endl;
            return false;
        } catch (...) { // TODO check for correct errors here
            std::cout << "static_mesh: " << mesh << " unknown exception" << std::endl;
            return false;
        }

        auto index = static_meshes_.size();
        if (dirty_ >= index)
            dirty_ = index;
        resource_map_[mesh] = index;
        gl_mesh.reference_count = 1;
        materials_.increment_reference(gl_mesh.material);
        static_meshes_.push_back(std::move(gl_mesh));

        return true;
    }

    bool static_mesh_cache::decrement_reference(resource::identifier mesh)
    {
        if (!is_cached(mesh))
            return false;
        auto index = resource_map_[mesh];
        if (static_meshes_[index].reference_count > 0) {
            static_meshes_[index].reference_count--;
            materials_.decrement_reference(static_meshes_[index].material);
        }
        return static_meshes_[index].reference_count == 0;
    }

    void static_mesh_cache::update()
    {
        static_assert(std::is_same<unsigned int, static_mesh::triangle::value_type>::value, "static mesh indices must be unsinged int");

        std::for_each(static_meshes_.begin() + static_cast<difference_type>(dirty_), static_meshes_.end(), [](static_mesh& mesh) {
            if (mesh.vertex_array == 0)
                GL_CHECK(glGenVertexArrays(1, &mesh.vertex_array));
            GL_CHECK(glBindVertexArray(mesh.vertex_array));

            if (mesh.vertex_buffer == 0)
                GL_CHECK(glGenBuffers(1, &mesh.vertex_buffer));

            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer));
            GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(static_mesh::vertex) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW));

            GL_CHECK(glEnableVertexAttribArray(0));
            GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(static_mesh::vertex), 0));

            GL_CHECK(glEnableVertexAttribArray(1));
            GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(static_mesh::vertex), reinterpret_cast<const void*>(sizeof(glm::vec3))));

            GL_CHECK(glEnableVertexAttribArray(2));
            GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(static_mesh::vertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3))));

            GL_CHECK(glEnableVertexAttribArray(3));
            GL_CHECK(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(static_mesh::vertex), reinterpret_cast<const void*>(3 * sizeof(glm::vec3))));

            if (mesh.index_buffer == 0)
                GL_CHECK(glGenBuffers(1, &mesh.index_buffer));

            GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer));
            GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int) * mesh.triangles.size(), mesh.triangles.data(), GL_STATIC_DRAW));
        });
        dirty_ = static_meshes_.size();

        // TODO free any meshes that have a reference count of zero
    }

    void sigma::opengl::static_mesh_cache::render(
        sigma::resource::identifier mesh_id,
        sigma::resource::identifier material_id,
        const glm::mat4 projection_matrix, const glm::mat4 view_matrix,
        const glm::mat4 model_matrix) const
    {
        if (resource_map_.count(mesh_id) != 0) {
            const auto& mesh = static_meshes_[resource_map_.at(mesh_id)];
            materials_.apply(material_id, projection_matrix, view_matrix, model_matrix);
            GL_CHECK(glBindVertexArray(mesh.vertex_array));
            GL_CHECK(glDrawElements(GL_TRIANGLES, 3 * mesh.triangles.size(), GL_UNSIGNED_INT, nullptr));
        }
    }

    void static_mesh_cache::render(resource::identifier mesh_id, const glm::mat4 projection_matrix, const glm::mat4 view_matrix, const glm::mat4 model_matrix) const
    {
        if (resource_map_.count(mesh_id) != 0) {
            const auto& mesh = static_meshes_[resource_map_.at(mesh_id)];
            materials_.apply(mesh.material, projection_matrix, view_matrix, model_matrix);
            GL_CHECK(glBindVertexArray(mesh.vertex_array));
            GL_CHECK(glDrawElements(GL_TRIANGLES, 3 * mesh.triangles.size(), GL_UNSIGNED_INT, nullptr));
        }
    }
}
}
