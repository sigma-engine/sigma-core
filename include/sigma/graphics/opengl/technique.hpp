#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/technique.hpp>

#include <sigma/graphics/opengl/texture.hpp>

#define TECHNIQUE_PTR(technique_mgr, x) technique_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class cubemap_manager;
    class shader_manager;
    struct instance_matrices;

    class technique {
    public:
        static constexpr const char* MODEL_MATRIX_NAME = "model_matrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "model_view_matrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normal_matrix";

        technique(shader_manager& shader_mgr, const sigma::graphics::technique& data);

        technique(technique&&) = default;

        technique& operator=(technique&&) = default;

        ~technique();

        void gather_uniform_locations(const sigma::graphics::technique_uniform_data& data);

        void set_uniform(const std::string& name, float value);

        void set_uniform(const std::string& name, const glm::vec2& value);

        void set_uniform(const std::string& name, const glm::vec3& value);

        void set_uniform(const std::string& name, const glm::vec4& value);

        void set_instance_matrices(instance_matrices* matrices) const;

        void bind(const sigma::graphics::technique_uniform_data& data);

        void bind(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, const sigma::graphics::technique_uniform_data& data, texture_unit first_texture_unit);

    protected:
        GLuint object_;
        GLint model_matrix_location_ = -1;
        GLint model_view_matrix_location_ = -1;
        GLint normal_matrix_location_ = -1;
        GLint in_image_location_ = -1;
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_depth_stencil_location_ = -1;
        GLint in_shadow_map0_location_ = -1;
        GLint in_shadow_map1_location_ = -1;
        GLint in_shadow_map2_location_ = -1;
        std::unordered_map<std::string, GLint> uniform_locations_;

    private:
        technique(const technique&) = delete;
        technique& operator=(const technique&) = delete;
    };

    class technique_manager {
    public:
        // TODO remove the use of unique_ptr

        technique_manager(shader_manager& shaders, resource::cache<graphics::technique>& technique_cache)
            : shaders_(shaders)
            , technique_cache_(technique_cache)
        {
        }

        resource::handle<graphics::technique> get(const graphics::technique_identifier& id)
        {
            return technique_cache_.get(id);
        }

        opengl::technique* acquire(const resource::handle<graphics::technique>& hndl)
        {
            // TODO not thread safe
            if (hndl.index >= techniques_.size())
                techniques_.resize(hndl.index + 1);

            if (techniques_[hndl.index] == nullptr)
                techniques_[hndl.index] = std::make_unique<technique>(shaders_, *technique_cache_.acquire(hndl));

            return techniques_.at(hndl.index).get();
        }

    private:
        shader_manager& shaders_;
        resource::cache<graphics::technique>& technique_cache_;
        std::vector<std::unique_ptr<technique>> techniques_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_HPP
