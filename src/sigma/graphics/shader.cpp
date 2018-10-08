#include <sigma/graphics/shader.hpp>

namespace sigma {
namespace graphics {
    shader::shader(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    void shader::add_source(shader_type type, std::vector<unsigned char>&& spirv, shader_schema&& schema)
    {
        type_ = type;
        spirv_ = std::move(spirv);
        schema_ = std::move(schema);
    }

    const shader_schema& shader::schema() const
    {
        return schema_;
    }
    void from_json(const nlohmann::json& j, shader_schema& schema)
    {
        auto ubos_it = j.find("ubos");
        if (ubos_it != j.end()) {
            for (const auto& j_ubo : *ubos_it) {
                std::string type_name = j_ubo["type"].get<std::string>();
                sigma::graphics::buffer_schema buffer_schema;
                buffer_schema = j["types"][type_name];
                buffer_schema.size = j_ubo["block_size"].get<size_t>();
                buffer_schema.descriptor_set = j_ubo["set"].get<size_t>();
                buffer_schema.binding_point = j_ubo["binding"].get<size_t>();
                buffer_schema.name = j_ubo["name"].get<std::string>();
                schema.buffers.push_back(buffer_schema);
            }
        }

        auto textures_it = j.find("textures");
        if (textures_it != j.end()) {
            for (const auto& j_texture : *textures_it) {
                std::string type = j_texture["type"].get<std::string>();
                shader_sampler sampler;
                sampler.name = j_texture["name"].get<std::string>();
                sampler.descriptor_set = j_texture["set"].get<size_t>();
                if (type == "sampler2D")
                    sampler.type = shader_sampler_type::SAMPLER2D;
                else if (type == "sampler2DArrayShadow")
                    sampler.type = shader_sampler_type::SAMPLER2D_ARRAY_SHADOW;
                else
                    throw std::runtime_error("Sampler Type: " + type + " is not supported!");
            }
        }
    }
}
}
