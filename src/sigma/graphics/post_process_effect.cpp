#include <sigma/graphics/post_process_effect.hpp>

#include <sigma/resource/manager.hpp>

namespace sigma {
namespace graphics {
    post_process_effect::post_process_effect(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, static_mesh_manager& static_meshes, const post_process_effect_data& data)
        : shader_technique(textures, cubemaps, shaders, data)
    {
        if (!has_shader(shader_type::vertex))
            set_shader(shader_type::vertex, shaders.get(resource::identifier{ "vertex", "fullscreen_quad" }));
        mesh_ = static_meshes.get(data.mesh);
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
