#ifndef SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/component.hpp>

#include <sigmafive/graphics/static_mesh.hpp>
#include <unordered_map>

namespace sigmafive {
namespace graphics {
    struct SIGMAFIVE_API static_mesh_instance {
        resource::identifier static_mesh;
    };

    class SIGMAFIVE_API static_mesh_instance_manager {
    public:
        static_mesh_instance_manager(graphics::static_mesh_cache& cache);

        static_mesh_instance_manager(const static_mesh_instance_manager&) = delete;

        virtual ~static_mesh_instance_manager();

        static_mesh_instance_manager&
        operator=(const static_mesh_instance_manager&) = delete;

        virtual bool has(entity e) const;

        virtual void add(entity e, resource::identifier mesh);

        virtual resource::identifier get(entity e) const;

        virtual void remove(entity e);

    private:
        graphics::static_mesh_cache& cache_;
        std::unordered_map<entity, resource::identifier> instances_;
    };
}
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::static_mesh_instance)

#endif // SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP
