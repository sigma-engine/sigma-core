#include <sigma/graphics/material.hpp>

#include <sigma/graphics/buffer.hpp>

#include <memory>

namespace sigma {
namespace graphics {
    void from_json(const nlohmann::json& j, material& mat)
    {
        if (auto ctx = mat.context().lock()) {
            auto shader_cache = ctx->cache<shader>();
            auto buffer_cache = ctx->cache<buffer>();

            mat.buffers_.clear();

            for (const auto& item : j.items()) {
                auto key = item.key();
                std::shared_ptr<shader> shader = nullptr;
                if (key == "vertex") {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    shader = shader_cache->get(shader_key);
                    mat.vertex_ = shader;
                } else if (key == "tessellation_control") {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    shader = shader_cache->get(shader_key);
                    mat.tessellation_control_ = shader;
                } else if (key == "tessellation_evaluation") {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    shader = shader_cache->get(shader_key);
                    mat.tessellation_evaluation_ = shader;
                } else if (key == "geometry") {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    shader = shader_cache->get(shader_key);
                    mat.geometry_ = shader;
                } else if (key == "fragment") {
                    auto shader_key = key / resource::key_type(item.value().get<std::string>());
                    shader = shader_cache->get(shader_key);
                    mat.fragment_ = shader;
                }

                if (shader) {
                    const auto& shader_schema = shader->schema();
                    for (const auto& buffer_schema : shader_schema.buffers) {
                        if (buffer_schema.binding_point >= mat.buffers_.size())
                            mat.buffers_.resize(buffer_schema.binding_point + 1);

                        auto buf = mat.buffers_[buffer_schema.binding_point];
                        if (buf == nullptr) {
                            resource::key_type buffer_key = mat.key() / buffer_schema.name;
                            buf = std::make_shared<buffer>(mat.context(), buffer_key, buffer_schema);
                            mat.buffers_[buffer_schema.binding_point] = buf;
                        } else if (!buf->merge(buffer_schema)) {
                            throw std::runtime_error("Buffer schema miss-match in material: " + mat.key().string());
                        }
                    }
                }
            }

            for (auto buffer : mat.buffers_) {
                if (buffer)
                    from_json(j, *buffer);
            }

            for (const auto& item : j.items()) {
                auto key = item.key();
                if (key == "vertex" || key == "tessellation_control" || key == "tessellation_evaluation" || key == "geometry" || key == "fragment") {
                    continue;
                } else if (key == "textures") {
                    // TODO: textures
                } else if (key == "cubemaps") {
                    // TODO: cubemaps
                }
            }
        } else {
            throw std::runtime_error("Could not lock context.");
        }
    }

    material::material(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    const std::vector<std::shared_ptr<buffer>>& material::buffers()
    {
        return buffers_;
    }
}
}
