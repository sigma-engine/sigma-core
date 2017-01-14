#include <sigma/graphics/shader_technique.hpp>

namespace sigma {
namespace graphics {
    shader_technique::shader_technique(const shader_technique_data& data)
    {
        for (const auto& shdr : data.shaders)
            shaders_[shdr.first] = resource::handle<graphics::shader>{ shdr.second };
		
		for (const auto& float_uniform : data.floats)
			floats_.push_back(std::make_pair(float_uniform.first, float_uniform.second));

		for (const auto& vec2_uniform : data.vec2s)
			vec2s_.push_back(std::make_pair(vec2_uniform.first, vec2_uniform.second));

		for (const auto& vec3_uniform : data.vec3s)
			vec3s_.push_back(std::make_pair(vec3_uniform.first, vec3_uniform.second));

		for (const auto& vec4_uniform : data.vec4s)
			vec4s_.push_back(std::make_pair(vec4_uniform.first, vec4_uniform.second));

        for (const auto& txt : data.textures)
            textures_.push_back(std::make_pair(txt.first, resource::handle<graphics::texture>{ txt.second }));

        for (const auto& cube : data.cubemaps)
            cubemaps_.push_back(std::make_pair(cube.first, resource::handle<graphics::cubemap>{ cube.second }));
    }

    bool shader_technique::has_shader(graphics::shader_type type) const
    {
        return shaders_.count(type) == 1;
    }

    resource::handle<graphics::shader>& shader_technique::shader(graphics::shader_type type)
    {
        return shaders_[type];
    }

    void shader_technique::set_shader(graphics::shader_type type, resource::handle<graphics::shader> shdr)
    {
        // TODO maybe if the shader handle passed in is not valid
        // and one of the other shaders have is valid
        // copy the manager into the invalid handle.
        if (shaders_[type] != shdr)
            shaders_[type] = shdr;
    }

    std::size_t shader_technique::texture_count() const
    {
        return textures_.size();
    }

    resource::handle<graphics::texture>& shader_technique::texture(std::size_t index)
    {
        return textures_[index].second;
    }

    void shader_technique::set_texture(std::size_t index, resource::handle<graphics::texture> txt)
    {
        // TODO maybe if the texture handle passed in is not valid
        // and one of the other textures have is valid
        // copy the manager into the invalid handle.
        if (textures_[index].second != txt)
            textures_[index].second = txt;
    }

    std::size_t shader_technique::cubemap_count() const
    {
        return cubemaps_.size();
    }

    resource::handle<graphics::cubemap>& shader_technique::cubemap(std::size_t index)
    {
        return cubemaps_[index].second;
    }

    void shader_technique::set_cubemap(std::size_t index, resource::handle<graphics::cubemap> cube)
    {
        // TODO maybe if the cubemap handle passed in is not valid
        // and one of the other cubemaps have is valid
        // copy the manager into the invalid handle.
        if (cubemaps_[index].second != cube)
            cubemaps_[index].second = cube;
    }
}
}
