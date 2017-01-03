#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/graphics/static_mesh.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    class SIGMA_API static_mesh_instance_manager {
    public:
        static_mesh_instance_manager() = default;

        virtual ~static_mesh_instance_manager() = default;

        virtual bool has(entity e) const;

        virtual void add(entity e, resource::handle<static_mesh> mesh);

        virtual resource::handle<static_mesh> get(entity e) const;

        virtual void remove(entity e);

    private:
        static_mesh_instance_manager(const static_mesh_instance_manager&) = delete;
        static_mesh_instance_manager& operator=(const static_mesh_instance_manager&) = delete;

        std::unordered_map<entity, resource::handle<static_mesh> > instances_;
    };
}
}

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
