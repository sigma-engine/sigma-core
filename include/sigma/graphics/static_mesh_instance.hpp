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
        resource::handle<static_mesh> mesh;
        bool cast_shadows;

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
namespace json {
    static bool from_json(std::shared_ptr<context> ctx, const Json::Value& source, graphics::static_mesh_instance& output)
    {
        if (source.isMember("mesh"))
            from_json(ctx, source["mesh"], output.mesh);
        
        if (source.isMember("cast_shadows"))
            from_json(ctx, source["cast_shadows"], output.cast_shadows);
        return true;
    }   
}
}

REGISTER_COMPONENT(sigma::graphics::static_mesh_instance)

#endif // SIGMA_GRAPHICS_STATIC_MESH_INSTANCE_HPP
