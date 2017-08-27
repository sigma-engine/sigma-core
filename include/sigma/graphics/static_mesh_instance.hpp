// TEMPLATE(generated.hpp)
#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect.hpp>
#include <sigma/resource/cache.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() static_mesh_instance {
        resource::handle<static_mesh> mesh;
        bool cast_shadows = true;

        // R_FIELD(derived) //TODO:RESOURCE remove this when new resource system is added.
        // std::unordered_map<std::size_t, resource::handle<material>> materials;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& mesh;
            ar& cast_shadows;
            //ar& materials;
        }
    };
}
}

#include <sigma/graphics/static_mesh_instance.generated.hpp>

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
