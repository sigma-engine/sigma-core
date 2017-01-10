#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/manager.hpp>

#include <boost/serialization/unordered_map.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {

    struct material_data {
        bool transparent = false;
        std::unordered_map<shader_type, resource::identifier> shaders;
        std::unordered_map<std::string, resource::identifier> textures;
        std::unordered_map<std::string, resource::identifier> cubemaps;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& transparent;
            ar& shaders;
            ar& textures;
            ar& cubemaps;
        }
    };

    class SIGMA_API material {
    public:
        using resource_data = material_data;

        material() = default;

        material(const material_data& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        virtual ~material() = default;

        bool is_transparent() const;

        bool has_shader(shader_type type) const;

        resource::handle<graphics::shader>& shader(shader_type type);

        void set_shader(shader_type type, resource::handle<graphics::shader> shdr);

		std::size_t texture_count() const;

        resource::handle<graphics::texture>& texture(std::size_t index);

        void set_texture(std::size_t index, resource::handle<graphics::texture> txt);

    protected:
        bool transparent_;
        std::unordered_map<shader_type, resource::handle<graphics::shader>> shaders_;
        std::vector<std::pair<std::string, resource::handle<graphics::texture>>> textures_;
        // TODO cubemap
        // std::vector<std::pair<std::string, resource::handle<cubemap>>> cubemaps_;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    using material_manager = resource::manager<material>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::material_data, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
