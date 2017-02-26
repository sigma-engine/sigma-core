#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/opengl/shader_technique.hpp>

#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/texture.hpp>

#define EFFECT_CONST_PTR(x) static_cast<const sigma::opengl::post_process_effect*>(x.get())
#define EFFECT_PTR(x) static_cast<sigma::opengl::post_process_effect*>(x.get())

namespace sigma {
namespace opengl {
    struct instance_matrices;

    class post_process_effect : public shader_technique<graphics::post_process_effect> {
    public:
        post_process_effect(const graphics::post_process_effect_data& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        void link();

        void bind() const;

        void apply() const;

    private:
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_depth_stencil_location_ = -1;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    class post_process_effect_manager : public shader_technique_manager<post_process_effect, graphics::post_process_effect_manager> {
    public:
        post_process_effect_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps, opengl::static_mesh_manager& meshes);

        virtual std::unique_ptr<graphics::post_process_effect> create(graphics::post_process_effect_data data) override;

    private:
        opengl::static_mesh_manager& meshes_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
