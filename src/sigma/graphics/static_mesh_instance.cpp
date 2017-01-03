#include <sigma/graphics/static_mesh_instance.hpp>

namespace sigma {
namespace graphics {
    bool static_mesh_instance_manager::has(entity e) const
    {
        return instances_.count(e) > 0;
    }

    void static_mesh_instance_manager::add(entity e, resource::handle<static_mesh> mesh)
    {
        instances_[e] = mesh;
    }

    resource::handle<static_mesh> static_mesh_instance_manager::get(entity e) const
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
