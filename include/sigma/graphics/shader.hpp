#ifndef SIGMA_GRAPHICS_SHADER_HPP
#define SIGMA_GRAPHICS_SHADER_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/buffer.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/resource.hpp>

#include <cereal/types/vector.hpp>

#include <nlohmann/json.hpp>

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

    struct shader_schema {
        std::vector<buffer_schema> buffers;
        std::vector<texture_schema> textures;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(buffers, textures);
        }
    };

    class shader : public resource::base_resource {
    public:
        shader(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        void add_source(shader_type type, std::vector<unsigned char>&& spirv, shader_schema&& schema);

        shader_type type() const;

        const shader_schema& schema() const;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(type_, schema_, spirv_);
        }

    private:
        shader_type type_;
        shader_schema schema_;
        std::vector<unsigned char> spirv_;
    };

    void to_json(nlohmann::json& j, const shader_schema& schema);

    void from_json(const nlohmann::json& j, shader_schema& schema);
}
}

REGISTER_RESOURCE(sigma::graphics::shader, shader, 0);
CEREAL_CLASS_VERSION(sigma::graphics::shader_schema, 0);

#endif // SIGMA_GRAPHICS_SHADER_HPP
