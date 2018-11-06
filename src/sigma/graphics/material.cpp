#include <sigma/graphics/material.hpp>

#include <sigma/graphics/buffer.hpp>

#include <memory>

namespace sigma {
namespace graphics {
    material::material(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    resource::handle_type<graphics::buffer> material::buffer(size_t index)
    {
        auto it = buffers_.find(index);
        if (it != buffers_.end())
        {
            return it->second;
        }
        return resource::handle_type<graphics::buffer>{};
    }

    void material::set_buffer(size_t index, resource::handle_type<graphics::buffer> buffer)
    {
        // TODO: check that the buffer matches the shaders
        buffers_[index] = std::move(buffer);
    }

    const std::unordered_map<size_t, resource::handle_type<graphics::buffer>>& material::buffers()
    {
        return buffers_;
    }

    resource::handle_type<graphics::shader> material::shader(shader_type type)
    {
        auto it = shaders_.find(type);
        if (it != shaders_.end())
        {
            return it->second;
        }
        return resource::handle_type<graphics::shader>{};
    }

    void material::set_shader(shader_type type, resource::handle_type<graphics::shader> shader)
    {
        // TODO: check that the buffers matches the shader
        shaders_[type] = std::move(shader);
    }

    const std::unordered_map<shader_type, resource::handle_type<graphics::shader>>& material::shaders()
    {
        return shaders_;
    }

    bool material::texture_binding_point(const std::string& sampler_name, size_t& index) const
    {
        bool found = false;
        for (const auto& shader : shaders_) {
            const auto& shader_schema = shader.second->schema();
            for (const auto& txt : shader_schema.textures) {
                if (txt.name == sampler_name) {
                    if (found && index != txt.binding_point)
                        throw std::runtime_error("Sampler '" + sampler_name + "' in '" + key().string() + "' binds to more than one binding point.");
                    found = true;
                    index = txt.binding_point;
                }
            }
        }

        return found;
    }

    void material::set_texture(size_t index, resource::handle_type<graphics::texture> texture)
    {
        // TODO: warn if texture will not be used
        textures_[index] = texture;
    }
}
}
