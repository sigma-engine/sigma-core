#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>

#include <nlohmann/json.hpp>

#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>

namespace sigma {
namespace graphics {
    class material;
    // void to_json(nlohmann::json& j, const material& mat);

    void from_json(const nlohmann::json& j, material& mat);

    class material : public resource::base_resource {
    public:
        material(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        const std::vector<std::shared_ptr<buffer>>& buffers();

        template <class Archive>
        void save(Archive& ar, const unsigned int version) const
        {
            bool has_vertex = vertex_ != nullptr;
            ar& has_vertex;
            if (has_vertex) {
                ar & vertex_->key();
            }

            bool has_tessellation_control = tessellation_control_ != nullptr;
            ar& has_tessellation_control;
            if (has_tessellation_control) {
                ar & tessellation_control_->key();
            }

            bool has_tessellation_evaluation = tessellation_evaluation_ != nullptr;
            ar& has_tessellation_evaluation;
            if (has_tessellation_evaluation) {
                ar & tessellation_evaluation_->key();
            }

            bool has_geometry = geometry_ != nullptr;
            ar& has_geometry;
            if (has_geometry) {
                ar & geometry_->key();
            }
            bool has_fragment = fragment_ != nullptr;
            ar& has_fragment;
            if (has_fragment) {
                ar & fragment_->key();
            }
        }

        template <class Archive>
        void load(Archive& ar, const unsigned int version)
        {
            if (auto ctx = context().lock()) {
                auto shader_cache = ctx->cache<shader>();
                resource::key_type key;

                bool has_vertex;
                ar& has_vertex;
                if (has_vertex) {
                    ar& key;
                    vertex_ = shader_cache->get(key);
                }

                bool has_tessellation_control;
                ar& has_tessellation_control;
                if (has_tessellation_control) {
                    ar& key;
                    tessellation_control_ = shader_cache->get(key);
                }

                bool has_tessellation_evaluation;
                ar& has_tessellation_evaluation;
                if (has_tessellation_evaluation) {
                    ar& key;
                    tessellation_evaluation_ = shader_cache->get(key);
                }

                bool has_geometry;
                ar& has_geometry;
                if (has_geometry) {
                    ar& key;
                    geometry_ = shader_cache->get(key);
                }

                bool has_fragment;
                ar& has_fragment;
                if (has_fragment) {
                    ar& key;
                    fragment_ = shader_cache->get(key);
                }
            } else {
                throw std::runtime_error("Could not lock context.");
            }
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()

    private:
        std::shared_ptr<shader> vertex_;
        std::shared_ptr<shader> tessellation_control_;
        std::shared_ptr<shader> tessellation_evaluation_;
        std::shared_ptr<shader> geometry_;
        std::shared_ptr<shader> fragment_;
        std::vector<std::shared_ptr<buffer>> buffers_;

        // friend void to_json(nlohmann::json& j, const material& mat);
        friend void from_json(const nlohmann::json& j, material& mat);
    };
}
}

REGISTER_RESOURCE(sigma::graphics::material, material, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP
