#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/post_process_effect.hpp>

#include <sigma/graphics/opengl/shader_technique.hpp>

#include <glad/glad.h>

#include <boost/filesystem/path.hpp>

// #define EFFECT_PTR(effect_mgr, x) static_cast<const sigma::opengl::post_process_effect*>(effect_mgr.acquire(x))
#define EFFECT_PTR(effect_mgr, x) static_cast<sigma::opengl::post_process_effect*>(effect_mgr.acquire(x))

namespace sigma {
namespace opengl {
    class cubemap_manager;
    class shader_manager;
    class static_mesh_manager;
    class texture_manager;

    class post_process_effect : public shader_technique<graphics::post_process_effect> {
    public:
        post_process_effect(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, static_mesh_manager& static_meshes, const graphics::post_process_effect_data& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        void link(opengl::shader_manager& shader_mgr);

        void bind(const opengl::texture_manager& texture_mgr, const opengl::cubemap_manager& cubemap_mgr) const;

        void apply(const static_mesh_manager& static_mesh_mgr) const;

    private:
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_depth_stencil_location_ = -1;
        GLint in_shadow_map_location_ = -1;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    class post_process_effect_manager : public sigma::graphics::post_process_effect_manager {
    public:
        post_process_effect_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps, opengl::static_mesh_manager& meshes);

        virtual std::unique_ptr<graphics::post_process_effect> create(graphics::post_process_effect_data data) override;

    private:
        opengl::shader_manager& shaders_;
        opengl::texture_manager& textures_;
        opengl::cubemap_manager& cubemaps_;
        opengl::static_mesh_manager& meshes_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
