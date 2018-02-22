#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/opengl/cubemap_manager.hpp>
#include <sigma/graphics/opengl/debug_draw_renderer.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader_manager.hpp>
#include <sigma/graphics/opengl/static_mesh_manager.hpp>
#include <sigma/graphics/opengl/technique_manager.hpp>
#include <sigma/graphics/opengl/texture_manager.hpp>
#include <sigma/graphics/opengl/uniform_buffer.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/shadow_block.hpp>
#include <sigma/graphics/standard_block.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace opengl {
    struct render_token {
        std::size_t program;
        static_mesh_manager::mesh_buffer buffer;
        std::size_t offset;
        std::size_t count;
        graphics::material* material;
        instance_matrices matrices;
    };

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
        GLint default_fbo_;

        GLuint gbuffer_fbo_;
        GLuint gbuffer_diffuse_texture_;
        GLuint gbuffer_normal_texture_;
        GLuint gbuffer_depth_stencil_texture_;

        GLuint accumulation_texture_;
        GLuint accumulation_fbo_;

        glm::ivec2 shadow_map_size_;
        GLuint shadow_array_texture_;
        std::vector<GLuint> shadow_framebuffers_;
        std::vector<frustum> cascade_frustums_;

        std::chrono::high_resolution_clock::time_point start_time_;
        graphics::standard_block standard_;
        uniform_buffer<graphics::standard_block> standard_uniform_buffer_;

        graphics::shadow_block shadow_;
        uniform_buffer<graphics::shadow_block> shadow_uniform_buffer_;

        instance_matrices matrices_;
        uniform_buffer<instance_matrices> matrices_buffer_;

        directional_light_block directional_light_;
        uniform_buffer<directional_light_block> directional_light_buffer_;

        point_light_block point_light_;
        uniform_buffer<point_light_block> point_light_buffer_;

        spot_light_block spot_light_;
        uniform_buffer<spot_light_block> spot_light_buffer_;

        opengl::texture_manager textures_;
        opengl::cubemap_manager cubemaps_;
        opengl::shader_manager shaders_;
        opengl::technique_manager techniques_;
        opengl::static_mesh_manager static_meshes_;
        resource::cache<graphics::material>& materials_;
        resource::cache<graphics::post_process_effect>& effects_;

        debug_draw_renderer debug_renderer_;
        std::vector<std::pair<glm::vec3, glm::mat4>> debug_frustums_;

        std::vector<render_token> geometry_pass_token_stream_;
        std::vector<render_token> shadow_map_token_stream_;

        void create_shadow_maps(std::size_t count, const glm::ivec2& size);

        void bind_for_shadow_read();

        void bind_for_shadow_write(unsigned int index);

        void destroy_shadow_maps();

        void create_geometry_buffer(const glm::ivec2& size);

        void bind_for_geometry_read();

        void bind_for_geometry_write();

        void destroy_geometry_buffer();

        void setup_view_projection(const glm::vec2& viewport_size, float fovy, float z_near, float z_far, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

        void geometry_pass(const graphics::view_port& viewport, const world_view_type& world, bool transparent);

        void light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void image_based_light_pass(const graphics::view_port& viewport);

        void analytical_light_setup();

        void directional_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void point_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void spot_light_pass(const graphics::view_port& viewport, const world_view_type& world);

        void render_to_shadow_map(const frustum& view_frustum, int index, const renderer::world_view_type& world, bool cast_shadows);

        void fill_render_token_stream(const frustum& view, const world_view_type& world, std::vector<render_token>& tokens, GLuint global_program = 0);

        void sort_render_token_stream(std::vector<render_token>& tokens);

        void render_token_stream(const std::vector<render_token>& tokens);

        std::pair<graphics::technique*, GLuint> bind_technique(
            const resource::handle<graphics::technique>& technique,
            const graphics::technique_uniform_data* data,
            GLenum first_texture_unit);

        void draw_effect_mesh(graphics::post_process_effect* effect);

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);
    };

    void debug_callback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
