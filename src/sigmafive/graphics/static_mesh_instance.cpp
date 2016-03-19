#include <sigmafive/graphics/static_mesh_instance.hpp>

namespace sigmafive {
namespace graphics {

    static_mesh_instance_manager::static_mesh_instance_manager(static_mesh_cache& cache)
        : cache_(cache)
    {
    }

    static_mesh_instance_manager::~static_mesh_instance_manager()
    {
        for (const auto& elm : instances_)
            cache_.decrement_reference(elm.second);
    }

    bool static_mesh_instance_manager::has(entity e) const
    {
        return instances_.count(e) > 0;
    }

    void static_mesh_instance_manager::add(entity e, resource::identifier mesh)
    {
        instances_[e] = mesh;
        cache_.increment_reference(mesh);
    }

    resource::identifier static_mesh_instance_manager::get(entity e) const
    {
        return instances_.at(e);
    }

    void static_mesh_instance_manager::remove(entity e)
    {
        auto it = instances_.find(e);
        if (it == instances_.end())
            return;
        cache_.decrement_reference(it->second);
        instances_.erase(it);
    }
}
}
