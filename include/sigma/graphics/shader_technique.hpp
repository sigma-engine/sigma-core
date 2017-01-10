#ifndef SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>

#include <boost/serialization/unordered_map.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {

    struct shader_technique_data {
        std::unordered_map<shader_type, resource::identifier> shaders;
        std::unordered_map<std::string, resource::identifier> textures;
        std::unordered_map<std::string, resource::identifier> cubemaps;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& shaders;
            ar& textures;
            ar& cubemaps;
        }
    };

    class SIGMA_API shader_technique {
    public:
        using resource_data = shader_technique_data;

        shader_technique() = default;

        shader_technique(const shader_technique_data& data);

        shader_technique(shader_technique&&) = default;

        shader_technique& operator=(shader_technique&&) = default;

        virtual ~shader_technique() = default;

        bool has_shader(shader_type type) const;

        resource::handle<graphics::shader>& shader(shader_type type);

        void set_shader(shader_type type, resource::handle<graphics::shader> shdr);

        std::size_t texture_count() const;

        resource::handle<graphics::texture>& texture(std::size_t index);

        void set_texture(std::size_t index, resource::handle<graphics::texture> txt);

    protected:
        std::unordered_map<shader_type, resource::handle<graphics::shader>> shaders_;
        std::vector<std::pair<std::string, resource::handle<graphics::texture>>> textures_;
        // TODO cubemap
        // std::vector<std::pair<std::string, resource::handle<cubemap>>> cubemaps_;

    private:
        shader_technique(const shader_technique&) = delete;
        shader_technique& operator=(const shader_technique&) = delete;
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::shader_technique_data, 1);

#endif // SIGMA_GRAPHICS_SHADER_TECHNIQUE_HPP
