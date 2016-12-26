#include <sigma/opengl/renderer.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    renderer::renderer(context* ctx, glm::ivec2 size)
        : graphics::renderer(ctx, size)
        , ctx_(ctx)
        , default_fbo_(size)
		, gbuffer_(size)
        , textures_(ctx_->textures())
        , shaders_(ctx_->shaders())
        , materials_(ctx_->materials(), textures_, shaders_)
        , static_meshes_(ctx_->static_meshes(), materials_)
    {
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    renderer::~renderer()
    {
    }

    void renderer::resize(glm::uvec2 size)
    {
        GL_CHECK(glViewport(0, 0, size.x, size.y));
    }

	void renderer::geometry_pass(const graphics::view_port& viewport) 
	{
		gbuffer_.bind(frame_buffer::target::DRAW);
		matrices_.projection_matrix = viewport.projection_matrix;
		matrices_.view_matrix = viewport.view_matrix;

		GL_CHECK(glEnable(GL_DEPTH_TEST));
		GL_CHECK(glDepthMask(GL_TRUE));

		GL_CHECK(glEnable(GL_CULL_FACE));
		GL_CHECK(glCullFace(GL_BACK));

		GL_CHECK(glDisable(GL_BLEND));
		
		GL_CHECK(glClearColor(0.8f, 0.8f, 0.8f, 1.0f));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		for (auto e : viewport.entities) { // TODO use a filter here
			if (viewport.static_mesh_instances.has(e) && viewport.transforms.has(e)) {
				const auto& txform = viewport.transforms.get(e);
				auto mesh = static_meshes_.get(viewport.static_mesh_instances.get(e));

				glm::mat4 model_matrix(1.0f);
				model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(model_matrix, txform.scale), txform.position);

				matrices_.model_view_matrix = viewport.view_matrix * model_matrix;
				matrices_.normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrices_.model_view_matrix)));
				mesh->render(&matrices_);
			}
		}

		GL_CHECK(glDepthMask(GL_FALSE));
		GL_CHECK(glDisable(GL_DEPTH_TEST));
	}

	void renderer::begin_light_pass()
	{
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendEquation(GL_FUNC_ADD));
		GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));

		default_fbo_.bind();

		gbuffer_.bind_textures();
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void renderer::light_pass(const graphics::view_port& viewport)
	{
		begin_light_pass();

		gbuffer_.read_buffer(frame_buffer::attachment::COLOR1);
		glm::ivec2 s = gbuffer_.size();
		glBlitFramebuffer(0, 0, s.x, s.y, 0, 0, s.x, s.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		end_light_pass();
	}

	void renderer::end_light_pass()
	{

	}

    void renderer::render(const graphics::view_port& viewport)
    {
		geometry_pass(viewport);
		light_pass(viewport);
    }
}
}

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)
