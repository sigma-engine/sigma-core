#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(std::shared_ptr<static_mesh> fullscreen_quad)
        : fullscreen_quad_(fullscreen_quad)
    {
    }

	void post_process_effect::apply(render_matrices *matrices, int texture_unit)
	{
		auto ind = glGetUniformLocation(object_, "input_image");
		GL_CHECK(glUniform1i(ind, texture_unit));
		fullscreen_quad_->render(matrices,this);
	}
}
}
