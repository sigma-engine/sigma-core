#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {
    mesh_part::mesh_part()
        : start_(0)
        , end_(0)
        , material_(nullptr)
    {
    }

    mesh_part::mesh_part(size_t start, size_t end, resource::handle_type<graphics::material> material)
        : start_(start)
        , end_(end)
        , material_(material)
    {
    }

    size_t mesh_part::start() const
    {
        return start_;
    }

    size_t mesh_part::end() const
    {
        return end_;
    }

    resource::handle_type<graphics::material> mesh_part::material() const
    {
        return material_;
    }

    static_mesh::static_mesh(std::weak_ptr<sigma::context> ctx, const resource::key_type& key)
        : resource::base_resource::base_resource(ctx, key)
    {
    }

    const std::vector<static_mesh::vertex>& static_mesh::vertices() const
    {
        return vertices_;
    }

    std::vector<static_mesh::vertex>& static_mesh::vertices()
    {
        return vertices_;
    }

    const std::vector<static_mesh::triangle>& static_mesh::triangles() const
    {
        return triangles_;
    }

    std::vector<static_mesh::triangle>& static_mesh::triangles()
    {
        return triangles_;
    }

    const std::vector<mesh_part>& static_mesh::parts() const
    {
        return parts_;
    }

    std::vector<mesh_part>& static_mesh::parts()
    {
        return parts_;
    }

    float static_mesh::radius() const
    {
        return radius_;
    }

    void static_mesh::set_radius(float r)
    {
        radius_ = r;
    }
}
}
