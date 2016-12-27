#include <sigma/graphics/post_process_effect.hpp>

namespace sigma {
namespace graphics {
    post_process_effect::post_process_effect()
        : mesh_("static_mesh://fullscreen_quad:plane")
    {
        shaders[shader_type::vertex] = resource::identifier{ "vertex://fullscreen_quad" };
    }

    resource::identifier post_process_effect::mesh() const
    {
        return mesh_;
    }

    void post_process_effect::set_mesh(resource::identifier mesh)
    {
        mesh_ = mesh;
    }

    post_process_effect_cache::post_process_effect_cache(boost::filesystem::path cache_directory, texture_cache& textures, shader_cache& shaders, static_mesh_cache& meshes)
        : shader_technique_cache<post_process_effect>(cache_directory, textures, shaders)
        , meshes_(meshes)
    {
    }

    bool post_process_effect_cache::increment_reference(resource::identifier resource_id)
    {
        if (shader_technique_cache<post_process_effect>::increment_reference(resource_id))
            return meshes_.increment_reference(get(resource_id).mesh());
        return true;
    }

    bool post_process_effect_cache::decrement_reference(resource::identifier resource_id)
    {
        if (shader_technique_cache<post_process_effect>::decrement_reference(resource_id))
            return meshes_.decrement_reference(get(resource_id).mesh());
        return false;
    }
}
}
