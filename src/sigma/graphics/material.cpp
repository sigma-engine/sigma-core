#include <sigma/graphics/material.hpp>

#include <sigma/graphics/buffer.hpp>

#include <memory>

namespace sigma {
namespace graphics {
    material::material(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    const std::unordered_map<size_t, resource::handle_type<buffer>>& material::buffers()
    {
        return buffers_;
    }

    bool material::binding_point(const std::string& sampler_name, size_t& index) const
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

    void from_json(const nlohmann::json& j, material& mat)
    {
        static const std::set<std::string> shader_keys = {
            "vertex", "tessellation_control", "tessellation_evaluation", "geometry", "fragment"
        };

        if (auto ctx = mat.context().lock()) {
            auto shader_cache = ctx->cache<shader>();
            auto buffer_cache = ctx->cache<buffer>();
            auto texture_cache = ctx->cache<texture>();

            mat.shaders_.clear();
            mat.buffers_.clear();
            mat.textures_.clear();

            for (const auto& item : j.items()) {
                auto key = item.key();
                if (shader_keys.count(key)) {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    auto shader = shader_cache->get(shader_key);

                    const auto& shader_schema = shader->schema();

                    // Validate and merge buffer schema
                    for (const auto& buffer_schema : shader_schema.buffers) {
                        auto it = mat.buffers_.find(buffer_schema.binding_point);
                        if (it == mat.buffers_.end()) {
                            resource::key_type buffer_key = mat.key() / buffer_schema.name;
                            auto buffer = buffer_cache->insert(buffer_key, std::make_shared<graphics::buffer>(mat.context(), buffer_key, buffer_schema));
                            mat.buffers_.emplace(std::make_pair(buffer_schema.binding_point, buffer));
                        } else if (!it->second->merge(buffer_schema)) {
                            throw std::runtime_error("Buffer schema miss-match in material: " + mat.key().string());
                        }
                    }

                    mat.shaders_[shader->type()] = shader;
                }
            }

            for (auto buffer : mat.buffers_) {
                if (buffer.second)
                    from_json(j, *buffer.second);
            }

            for (const auto& item : j.items()) {
                const auto& key = item.key();
                const auto& value = item.value();
                if (key == "textures") {
                    for (const auto& texture_j : value.items()) {
                        size_t index;
                        if (mat.binding_point(texture_j.key(), index)) {
                            mat.textures_[index] = texture_cache->get(texture_j.value().get<std::string>());
                        }
                    }
                } else if (key == "cubemaps") {
                    // TODO: cubemaps
                }
            }
        } else {
            throw std::runtime_error("Could not lock context.");
        }
    }

}
}
