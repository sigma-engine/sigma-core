#include <sigma/graphics/opengl/post_process_effect.hpp>

#include <sigma/graphics/opengl/static_mesh.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(technique_manager& technique_mgr, static_mesh_manager& static_meshes, const graphics::post_process_effect& data)
        : data(data)
    {
        TECHNIQUE_PTR(technique_mgr, data.technique_id)->gather_uniform_locations(data);
    }
}
}
