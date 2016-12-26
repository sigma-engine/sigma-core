#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/shader_technique.hpp>

namespace sigma {
namespace opengl {
	struct render_matrices;

    class post_process_effect : public shader_technique {
    public:
        post_process_effect(std::shared_ptr<static_mesh> fullscreen_quad);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

		void apply(render_matrices *matrices, int texture_unit);

    private:
		std::shared_ptr<static_mesh> fullscreen_quad_;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
