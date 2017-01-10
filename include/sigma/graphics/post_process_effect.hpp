#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/utility.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {
    // TODO copy and pasted
    struct post_process_effect_data {
        std::unordered_map<shader_type, resource::identifier> shaders{ { shader_type::vertex, resource::identifier{ "vertex://fullscreen_quad" } } };
        std::unordered_map<std::string, resource::identifier> textures;
        std::unordered_map<std::string, resource::identifier> cubemaps;
        resource::identifier mesh = "static_mesh://fullscreen_quad:plane";

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
            ar& cubemaps;
            ar& mesh;
        }
    };

    // TODO copy and pasted
    class SIGMA_API post_process_effect {
    public:
        using resource_data = post_process_effect_data;

        post_process_effect() = default;

        post_process_effect(const post_process_effect_data& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        virtual ~post_process_effect() = default;

        bool has_shader(shader_type type) const;

        resource::handle<graphics::shader>& shader(shader_type type);

        void set_shader(shader_type type, resource::handle<graphics::shader> shdr);

        std::size_t texture_count() const;

        resource::handle<graphics::texture>& texture(std::size_t index);

        void set_texture(std::size_t index, resource::handle<graphics::texture> txt);

        resource::handle<static_mesh>& mesh();

        void set_mesh(resource::handle<static_mesh> mesh);

    protected:
        resource::handle<static_mesh> mesh_;
        std::unordered_map<shader_type, resource::handle<graphics::shader>> shaders_;
        std::vector<std::pair<std::string, resource::handle<graphics::texture>>> textures_;
        // TODO cubemap
        // std::vector<std::pair<std::string, resource::handle<cubemap>>> cubemaps_;

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    using post_process_effect_manager = resource::manager<post_process_effect>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::post_process_effect_data, 1);

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
