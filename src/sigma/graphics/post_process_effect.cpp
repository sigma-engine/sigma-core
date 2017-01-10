#include <sigma/graphics/post_process_effect.hpp>

namespace sigma {
namespace graphics {
    // TODO copy and pasted
    post_process_effect::post_process_effect(const post_process_effect_data& data)
    {
        for (const auto& shdr : data.shaders)
            shaders_[shdr.first] = resource::handle<graphics::shader>{ shdr.second };
        for (const auto& txt : data.textures)
            textures_.push_back(std::make_pair(txt.first, resource::handle<graphics::texture>{ txt.second }));
        // TODO cubemap
        // for (const auto& cube : data.cubemaps)
        //     textures_.push_back(std::make_pair(cube.first, resource::handle<cubemap>{ cube.second }));

		mesh_ = resource::handle<static_mesh>{ data.mesh };
    }

    bool post_process_effect::has_shader(graphics::shader_type type) const
    {
        return shaders_.count(type) == 1;
    }

    resource::handle<graphics::shader>& post_process_effect::shader(graphics::shader_type type)
    {
        return shaders_[type];
    }

    void post_process_effect::set_shader(graphics::shader_type type, resource::handle<graphics::shader> shdr)
    {
        // TODO maybe if the shader handle passed in is not valid
        // and one of the other shaders have is valid
        // copy the manager into the invalid handle.
        if (shaders_[type] != shdr)
            shaders_[type] = shdr;
    }

    std::size_t post_process_effect::texture_count() const
    {
        return textures_.size();
    }

    resource::handle<graphics::texture>& post_process_effect::texture(std::size_t index)
    {
        return textures_[index].second;
    }

    void post_process_effect::set_texture(std::size_t index, resource::handle<graphics::texture> txt)
    {
        // TODO maybe if the texture handle passed in is not valid
        // and one of the other textures have is valid
        // copy the manager into the invalid handle.
        if (textures_[index].second != txt)
            textures_[index].second = txt;
    }

    resource::handle<static_mesh>& post_process_effect::mesh()
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
