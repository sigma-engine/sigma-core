#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/opengl/shader_technique.hpp>

#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/texture.hpp>

#define EFFECT_CONST_PTR(x) static_cast<const sigma::opengl::post_process_effect*>(x.get())
#define EFFECT_PTR(x) static_cast<sigma::opengl::post_process_effect*>(x.get())

namespace sigma {
namespace opengl {
    struct render_matrices;

    class post_process_effect : public graphics::post_process_effect, public shader_technique {
    public:
        post_process_effect(resource::handle<graphics::static_mesh> mesh);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        void link();

        void apply(render_matrices* matrices);

    private:
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_image_location_ = -1;
        GLint in_depth_stencil_location_ = -1;

        resource::handle<graphics::static_mesh> mesh_;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    class post_process_effect_manager : public graphics::post_process_effect_manager {
    public:
        post_process_effect_manager(boost::filesystem::path cache_directory, opengl::texture_manager& textures, opengl::shader_manager& shaders, opengl::static_mesh_manager& meshes);

        virtual std::unique_ptr<graphics::post_process_effect> load(graphics::post_process_effect_data data, boost::archive::binary_iarchive& ia) override;

    private:
        opengl::texture_manager& textures_;
        opengl::shader_manager& shaders_;
        opengl::static_mesh_manager& meshes_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
