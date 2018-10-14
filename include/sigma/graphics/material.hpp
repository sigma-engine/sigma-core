#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>

#include <cereal/types/unordered_map.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    class material : public resource::base_resource {
    public:
        material(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        resource::handle_type<graphics::buffer> buffer(size_t index);

        void set_buffer(size_t index, resource::handle_type<graphics::buffer> buffer);

        const std::unordered_map<size_t, resource::handle_type<graphics::buffer>>& buffers();

        resource::handle_type<graphics::shader> shader(shader_type type);

        void set_shader(shader_type type, resource::handle_type<graphics::shader> shader);

        const std::unordered_map<shader_type, resource::handle_type<graphics::shader>>& shaders();

        bool texture_binding_point(const std::string& sampler_name, size_t& index) const;

        void set_texture(size_t index, resource::handle_type<graphics::texture> texture);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(shaders_, textures_, buffers_);
        }
    private:
        std::unordered_map<shader_type, resource::handle_type<graphics::shader>> shaders_;
        std::unordered_map<size_t, resource::handle_type<graphics::texture>> textures_;
        std::unordered_map<size_t, resource::handle_type<graphics::buffer>> buffers_;
    };
}
}

REGISTER_RESOURCE(sigma::graphics::material, material, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
