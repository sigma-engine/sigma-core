#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>

#include <cereal/types/unordered_map.hpp>

#include <nlohmann/json.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    class material : public resource::base_resource {
    public:
        material(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        const std::unordered_map<size_t, resource::handle_type<buffer>>& buffers();

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(shaders_, textures_, buffers_);
        }

    private:
        bool binding_point(const std::string& sampler_name, size_t& index) const;

        std::unordered_map<shader_type, resource::handle_type<shader>> shaders_;
        std::unordered_map<size_t, resource::handle_type<texture>> textures_;
        std::unordered_map<size_t, resource::handle_type<buffer>> buffers_;

        friend void to_json(nlohmann::json& j, const material& mat);
        friend void from_json(const nlohmann::json& j, material& mat);
    };

    void to_json(nlohmann::json& j, const material& mat);

    void from_json(const nlohmann::json& j, material& mat);
}
}

REGISTER_RESOURCE(sigma::graphics::material, material, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
