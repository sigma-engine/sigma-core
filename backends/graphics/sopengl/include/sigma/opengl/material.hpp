#ifndef SIGMA_FIVE_OPENGL_MATERIAL_HPP
#define SIGMA_FIVE_OPENGL_MATERIAL_HPP

#include <sigma/opengl/config.hpp>

#include <sigma/graphics/material.hpp>

#include <chrono>
#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <glm/mat4x4.hpp>

#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>

namespace sigma {
namespace opengl {
    struct material : public graphics::material {
        material() = default;

        material(const material&) = delete;

        material(material&&) = default;

        material& operator=(const material&) = delete;

        material& operator=(material&&) = default;

        ~material() = default;

        GLuint object = 0;

        // TOOD move these to uniform buffers.
        GLint projection_matrix_location = -1;
        GLint view_matrix_location = -1;
        GLint model_matrix_location = -1;
        GLint model_view_matrix_location = -1;
        GLint normal_matrix_location = -1;

        GLint time_location = -1;

        std::size_t reference_count = 0;

        std::vector<GLint> texture_locations;

        template <typename T>
        void set_uniform(const std::string& name, const T& value) const
        {
            set_uniform(get_uniform_location(name), value);
        }

        GLint get_uniform_location(const std::string& name) const;

        void set_uniform(GLint location, const glm::mat4& value) const;
    };

    class SIGMA_FIVE_OPENGL_API material_cache : public graphics::material_cache {
    public:
        static constexpr const char* PROJECTION_MATRIX_NAME = "projectionMatrix";
        static constexpr const char* VIEW_MATRIX_NAME = "viewMatrix";
        static constexpr const char* MODEL_MATRIX_NAME = "modelMatrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "modelViewMatrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normalMatrix";
        static constexpr const char* TIME_NAME = "time";

        material_cache(shader_cache& shaders, texture_cache& textures);

        material_cache(const material_cache&) = delete;

        material_cache(material_cache&&) = default;

        material_cache& operator=(const material_cache&) = delete;

        material_cache& operator=(material_cache&&) = default;

        virtual ~material_cache();

        virtual bool is_cached(resource::identifier material) const override;

        virtual bool increment_reference(resource::identifier material) override;

        virtual bool decrement_reference(resource::identifier material) override;

        virtual void apply(resource::identifier material_id, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::mat4& modelMatrix);

        void update();

    private:
        using size_type = std::vector<material>::size_type;
        using difference_type = std::vector<material>::difference_type;

        boost::filesystem::path cache_directory_;
        std::unordered_map<resource::identifier, size_type> resource_map_;

        shader_cache& shaders_;
        texture_cache& textures_;
        std::vector<material> materials_;
        size_type dirty_;

        std::chrono::high_resolution_clock::time_point start_time_;
        std::chrono::duration<float> elpased_time_;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_MATERIAL_HPP
