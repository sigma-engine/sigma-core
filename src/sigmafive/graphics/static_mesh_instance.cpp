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

    void static_mesh_instance_manager::add(entity e, resource::identifier mesh)
    {
        cache_.increment_reference(mesh);
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
