#include <sigma/graphics/opengl/post_process_effect.hpp>

#include <sigma/graphics/opengl/static_mesh.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(technique_manager& technique_mgr, static_mesh_manager& static_meshes, const graphics::post_process_effect& data)
        : technique(technique_mgr.get(data.technique_id))
        , mesh(static_meshes.get(data.mesh))
        , data(data)
    {
        TECHNIQUE_PTR(technique_mgr, technique)->gather_uniform_locations(data);
    }
}
}
