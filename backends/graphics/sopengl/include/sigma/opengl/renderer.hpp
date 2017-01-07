#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/graphics/renderer.hpp>

#include <sigma/opengl/config.hpp>
#include <sigma/opengl/frame_buffer.hpp>
#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/material.hpp>
#include <sigma/opengl/post_process_effect.hpp>
#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/uniform_buffer.hpp>

namespace sigma {

namespace opengl {
    struct internal_point_light {
        glm::vec4 color_intensity;
        glm::vec4 position_radius;
    };

    struct internal_directional_light {
        glm::vec4 color_intensity;
        glm::vec3 direction;
    };

    struct mesh_instance_data {
        glm::mat4 model_matrix;
    };

    struct mesh_bucket {
        resource::handle<graphics::static_mesh> active_mesh;
        std::vector<mesh_instance_data> instances;
    };

    struct material_bucket {
        resource::handle<graphics::material> active_material;
        std::vector<mesh_bucket> mesh_buckets;
    };

    class renderer : public graphics::renderer {
    public:
        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual graphics::texture_manager& textures() override;

        virtual graphics::shader_manager& shaders() override;

        virtual graphics::material_manager& materials() override;

        virtual graphics::static_mesh_manager& static_meshes() override;

        virtual graphics::post_process_effect_manager& effects() override;

        virtual std::shared_ptr<graphics::scene> create_scene() override;

        virtual void resize(glm::uvec2 size) override;

        void geometry_pass(const graphics::view_port& viewport, bool transparent);

        void light_pass(const graphics::view_port& viewport);

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);

        virtual void render(const graphics::view_port& viewport) override;

    private:
        static const resource::identifier POINT_LIGHT_EFFECT;
        static const resource::identifier POINT_LIGHT_STENCIL_EFFECT;
        static const resource::identifier DIRECTIONAL_LIGHT_EFFECT;
        static const resource::identifier VIGNETTE_EFFECT;
        static const resource::identifier GAMMA_CONVERSION_EFFECT;

        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        default_frame_buffer default_fbo_;
        geometry_buffer gbuffer_;
        standard_uniforms standard_uniform_data_;
        //uniform_buffer<standard_uniforms> standard_uniforms_;

        opengl::texture_manager textures_;
        opengl::shader_manager shaders_;
        opengl::material_manager materials_;
        opengl::static_mesh_manager static_meshes_;
        opengl::post_process_effect_manager effects_;

        resource::handle<graphics::post_process_effect> point_light_effect_;
        resource::handle<graphics::post_process_effect> point_light_stencil_effect_;

        resource::handle<graphics::post_process_effect> directional_light_effect_;

        resource::handle<graphics::post_process_effect> texture_blit_effect_;

        resource::handle<graphics::post_process_effect> vignette_effect_;
        resource::handle<graphics::post_process_effect> gamma_conversion_;

        bool point_light_buffer_filled_ = false;
        std::vector<internal_point_light> internal_point_lights_;
        GLuint point_light_data_buffer_ = -1;

        bool directional_light_buffer_filled_ = false;
        std::vector<internal_directional_light> internal_directional_lights_;
        GLuint directional_light_data_buffer_ = -1;

        bool material_buckets_filled_ = false;
        std::vector<material_bucket> material_buckets_;
        GLuint mesh_instance_data_buffer_ = -1;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
