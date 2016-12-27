#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/opengl/config.hpp>
#include <sigma/opengl/frame_buffer.hpp>
#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/material_manager.hpp>
#include <sigma/opengl/post_process_effect_manager.hpp>
#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader_manager.hpp>
#include <sigma/opengl/static_mesh_manager.hpp>
#include <sigma/opengl/texture_manager.hpp>

#include <sigma/graphics/renderer.hpp>

namespace sigma {

namespace opengl {
    class renderer : public graphics::renderer {
    public:
        renderer(context* ctx, glm::ivec2 size);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) override;

        void geometry_pass(const graphics::view_port& viewport);

        void begin_light_pass();

        void light_pass(const graphics::view_port& viewport);

        void end_light_pass();

        virtual void render(const graphics::view_port& viewport) override;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        static const resource::identifier TEXTURE_BLIT_EFFECT;
        static const resource::identifier DIRECTIONAL_LIGHT_EFFECT;
        static const resource::identifier POINT_LIGHT_EFFECT;

        context* ctx_;
        default_frame_buffer default_fbo_;
        geometry_buffer gbuffer_;

        texture_manager textures_;
        shader_manager shaders_;
        material_manager materials_;
        static_mesh_manager static_meshes_;
        post_process_effect_manager effects_;
        render_matrices matrices_;

        std::shared_ptr<post_process_effect> fullscreen_blit_;
        std::shared_ptr<post_process_effect> directional_light_effect_;
        std::shared_ptr<post_process_effect> point_light_effect_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
