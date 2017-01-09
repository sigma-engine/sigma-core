#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/resource/manager.hpp>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>

#include <string>
#include <unordered_map>

namespace sigma {
namespace graphics {

    struct material_data {
        bool transparent = false;
        std::unordered_map<shader_type, resource::identifier> shaders;
        std::unordered_map<std::string, resource::identifier> textures;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& transparent;
            ar& shaders;
            ar& textures;
        }
    };

    class SIGMA_API material {
    public:
        using resource_data = material_data;

        material() = default;

        material(const material_data &data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        virtual ~material() = default;

        bool is_transparent() const;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;

        material_data data_;
    };

    using material_manager = resource::manager<material>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::material_data, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
