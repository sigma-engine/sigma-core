#ifndef SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect.hpp>
#include <sigma/resource/cache.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() static_mesh_instance {
        BOOST_HANA_DEFINE_STRUCT(static_mesh_instance,
            (resource::handle<static_mesh>, mesh),
            (bool, cast_shadows));

        static_mesh_instance(const resource::handle<static_mesh>& mesh = {}, bool cast_shadows = true)
            : mesh{ mesh }
            , cast_shadows{ true }
        {
        }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(mesh,cast_shadows);
        }
    };
}
}

REGISTER_COMPONENT(sigma::graphics::static_mesh_instance)

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
