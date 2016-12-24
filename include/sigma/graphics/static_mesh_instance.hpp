#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/config.hpp>

#include <unordered_map>

#include <sigma/component.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect/reflect.hpp>

namespace sigma {
namespace graphics {
    RCLASS()
    struct SIGMA_API static_mesh_instance {
        resource::identifier static_mesh;
    };

    class SIGMA_API static_mesh_instance_manager {
    public:
        static_mesh_instance_manager(graphics::static_mesh_cache& cache);

        static_mesh_instance_manager(const static_mesh_instance_manager&) = delete;

        virtual ~static_mesh_instance_manager();

        static_mesh_instance_manager& operator=(const static_mesh_instance_manager&) = delete;

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

SIGMA_COMPONENT(sigma::graphics::static_mesh_instance)

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
