#include <sigmafive/graphics/static_mesh.hpp>

namespace sigmafive {
namespace graphics {

    void static_mesh::set_data(std::vector<static_mesh::vertex> vertices,
        std::vector<static_mesh::triangle> triangles)
    {
        //TODO add checks
        vertices_ = std::move(vertices);
        triangles_ = std::move(triangles);
    }

    const std::vector<static_mesh::vertex>& static_mesh::vertices() const { return vertices_; }

    const std::vector<static_mesh::triangle>& static_mesh::triangles() const { return triangles_; }

    resource::identifier static_mesh::get_material() const
    {
        return material_;
    }

    void static_mesh::set_material(resource::identifier material)
    {
        material_ = material;
    }
}
}
