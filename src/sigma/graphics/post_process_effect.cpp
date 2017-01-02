#include <sigma/graphics/post_process_effect.hpp>

namespace sigma {
namespace graphics {
    post_process_effect::post_process_effect()
        : mesh_("static_mesh://fullscreen_quad:plane")
    {
        shaders[shader_type::vertex] = resource::identifier{ "vertex://fullscreen_quad" };
    }

    static_mesh_cache::instance post_process_effect::mesh() const
    {
        return mesh_;
    }

    void post_process_effect::set_mesh(static_mesh_cache::instance mesh)
    {
        mesh_ = mesh;
    }
}
}
