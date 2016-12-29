#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/opengl/shader_technique.hpp>

namespace sigma {
namespace opengl {
    class static_mesh;

    struct render_matrices;

    class post_process_effect : public shader_technique {
    public:
        post_process_effect(std::shared_ptr<static_mesh> mesh);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        void link();

        void apply(render_matrices* matrices);

    private:
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_image_location_ = -1;

        std::shared_ptr<static_mesh> mesh_;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
