#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/buffer.hpp>
#include <sigma/resource/resource.hpp>

#include <nlohmann/json.hpp>

#include <boost/serialization/vector.hpp>

#include <vector>

namespace sigma {
namespace graphics {
    enum class shader_type : size_t {
        vertex,
        tessellation_control,
        tessellation_evaluation,
        geometry,
        fragment,
        header
    };

    enum class shader_sampler_type {
        SAMPLER2D,
        SAMPLER2D_ARRAY_SHADOW
    };

    struct shader_sampler {
        shader_sampler_type type;
        size_t descriptor_set;
        std::string name;
    };

    struct shader_schema {
        std::vector<buffer_schema> buffers;
        std::vector<shader_sampler_type> textures;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& buffers;
            ar& textures;
        }
    };

    class shader : public resource::base_resource {
    public:
        shader(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        void add_source(shader_type type, std::vector<unsigned char>&& spirv, shader_schema&& schema);

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& type_;
            ar& spirv_;
            ar& schema_;
        }

    private:
        size_t id_;
        shader_type type_;
        shader_schema schema_;
        std::vector<unsigned char> spirv_;
    };

    // void to_json(nlohmann::json& j, const shader_schema& schema);

    void from_json(const nlohmann::json& j, shader_schema& schema);
}
}

REGISTER_RESOURCE(sigma::graphics::shader, shader, 0);
BOOST_CLASS_VERSION(sigma::graphics::shader_schema, 0);

#endif // SIGMA_GRAPHICS_SHADER_HPP
