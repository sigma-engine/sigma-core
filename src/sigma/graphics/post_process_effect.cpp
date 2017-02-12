#include <sigma/graphics/post_process_effect.hpp>

namespace sigma {
namespace graphics {
    post_process_effect::post_process_effect(const post_process_effect_data& data)
        : shader_technique(data)
    {
        if (!has_shader(shader_type::vertex))
            set_shader(shader_type::vertex, resource::identifier{ "vertex", "fullscreen_quad" });
        mesh_ = resource::handle<static_mesh>{ data.mesh };
    }

    resource::handle<static_mesh>& post_process_effect::mesh()
    {
        return mesh_;
    }

    const resource::handle<static_mesh>& post_process_effect::mesh() const
    {
        return mesh_;
    }

    void post_process_effect::set_mesh(resource::handle<static_mesh> mesh)
    {
        // TODO maybe if the mesh handle passed in is not valid
        // and the old one is valid
        // copy the manager into the invalid handle.
        if (mesh_ != mesh)
            mesh_ = mesh;
    }
}
}
