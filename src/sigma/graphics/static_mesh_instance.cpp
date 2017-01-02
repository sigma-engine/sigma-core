#include <sigma/graphics/static_mesh_instance.hpp>

namespace sigma {
namespace graphics {
    static_mesh_instance_manager::static_mesh_instance_manager(static_mesh_cache& cache)
        : cache_(cache)
    {
    }

    static_mesh_instance_manager::~static_mesh_instance_manager()
    {
    }

    bool static_mesh_instance_manager::has(entity e) const
    {
        return instances_.count(e) > 0;
    }

    void static_mesh_instance_manager::add(entity e, static_mesh_cache::instance mesh)
    {
        if(!mesh)
            mesh = cache_.get(mesh);
        instances_[e] = mesh;
    }

    static_mesh_cache::instance static_mesh_instance_manager::get(entity e) const
    {
        return instances_.at(e);
    }

    void static_mesh_instance_manager::remove(entity e)
    {
        auto it = instances_.find(e);
        if (it == instances_.end())
            return;
        instances_.erase(it);
    }
}
}
