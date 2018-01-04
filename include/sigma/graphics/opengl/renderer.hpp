#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/debug_draw_renderer.hpp>
#include <sigma/graphics/opengl/frame_buffer.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/material.hpp>
#include <sigma/graphics/opengl/post_process_effect.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/shadow_buffer.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/technique.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/uniform_buffer.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/shadow_block.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sigma {

namespace opengl {
    void calculate_cascade_frustums(const frustum& view_frustum, std::vector<frustum>& cascade_frustums);

    class SIGMA_API renderer : public graphics::renderer {
    public:
        bool save_frustums = false;

        renderer(glm::ivec2 size, graphics::renderer::context_view_type ctx);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) override;

        virtual void render(const graphics::view_port& viewport, const world_view_type& world) override;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        int loader_status_;
        glm::vec2 size_;
        frame_buffer default_fbo_;
        geometry_buffer gbuffer_;
        shadow_buffer sbuffer_;
        std::vector<frustum> cascade_frustums_;

        std::chrono::high_resolution_clock::time_point start_time_;
        graphics::standard_block standard_;
        uniform_buffer<graphics::standard_block> standard_uniform_buffer_;

        graphics::shadow_block shadow_;
        uniform_buffer<graphics::shadow_block> shadow_uniform_buffer_;

        opengl::texture_manager textures_;
        opengl::cubemap_manager cubemaps_;
        opengl::shader_manager shaders_;
        opengl::technique_manager techniques_;
        opengl::material_manager materials_;
        opengl::static_mesh_manager static_meshes_;
        opengl::post_process_effect_manager effects_;

        debug_draw_renderer debug_renderer_;
        std::vector<std::pair<glm::vec3, glm::mat4>> debug_frustums_;

        void begin_effect(opengl::post_process_effect* effect);

        void end_effect(opengl::post_process_effect* effect);

        void setup_view_projection(const glm::vec2& viewport_size, float fovy, float z_near, float z_far, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

        void geometry_pass(const graphics::view_port& viewport, const world_view_type& world, bool transparent);

        void light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void image_based_light_pass(const graphics::view_port& viewport);

        void analytical_light_setup();

        void directional_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void point_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void spot_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void render_to_shadow_map(const frustum& view_frustum, int index, const renderer::world_view_type& world, bool cast_shadows);

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
