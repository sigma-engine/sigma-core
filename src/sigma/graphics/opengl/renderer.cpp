#include <sigma/graphics/opengl/renderer.hpp>

#include <glad/glad.h>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>

namespace sigma {
namespace opengl {
    renderer::renderer(glm::ivec2 size)
        : graphics::renderer(size)
        , loader_status_(gladLoadGL())
        , default_fbo_(size)
        , gbuffer_(size)
        , shaders_(boost::filesystem::current_path() / ".." / "data")
        , textures_(boost::filesystem::current_path() / ".." / "data")
        , cubemaps_(boost::filesystem::current_path() / ".." / "data")
        , materials_(boost::filesystem::current_path() / ".." / "data", shaders_, textures_, cubemaps_)
        , static_meshes_(boost::filesystem::current_path() / ".." / "data", materials_)
        , effects_(boost::filesystem::current_path() / ".." / "data", shaders_, textures_, cubemaps_, static_meshes_)
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        //standard_uniforms_.set_binding_point(shader_technique::STANDARD_UNIFORM_BLOCK_BINDING);

        image_based_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/image_based");
        point_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/point");
        directional_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/directional");
        spot_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/spot");
        texture_blit_effect_ = effects_.get("post_process_effect://pbr/deffered/texture_blit");

        shadow_material_ = materials_.get("material://shadow");

        vignette_effect_ = effects_.get("post_process_effect://vignette");
        gamma_conversion_ = effects_.get("post_process_effect://pbr/deffered/gamma_conversion");
    }

    renderer::~renderer()
    {
    }

    graphics::texture_manager& renderer::textures()
    {
        return textures_;
    }

    graphics::cubemap_manager& renderer::cubemaps()
    {
        return cubemaps_;
    }

    graphics::shader_manager& renderer::shaders()
    {
        return shaders_;
    }

    graphics::material_manager& renderer::materials()
    {
        return materials_;
    }

    graphics::static_mesh_manager& renderer::static_meshes()
    {
        return static_meshes_;
    }

    graphics::post_process_effect_manager& renderer::effects()
    {
        return effects_;
    }

    void renderer::resize(glm::uvec2 size)
    {
        GL_CHECK(glViewport(0, 0, size.x, size.y));
    }

    /*void renderer::point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius)
	{
		// http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
		if (glm::length(view_space_position) > 1.1 * radius) {
			gbuffer_.bind_for_stencil_pass();

			GL_CHECK(glEnable(GL_STENCIL_TEST));
			GL_CHECK(glClearStencil(4));
			GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));

			GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
			GL_CHECK(glStencilFunc(GL_GEQUAL, 6, 0xFF));

			GL_CHECK(glDepthFunc(GL_GEQUAL));
			GL_CHECK(glCullFace(GL_FRONT));

			EFFECT_PTR(point_light_stencil_effect_)->apply(&matrices_);

			GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
			GL_CHECK(glStencilFunc(GL_LEQUAL, 5, 0xFF));

			GL_CHECK(glDepthFunc(GL_LEQUAL));
			GL_CHECK(glCullFace(GL_BACK));
		}
	}*/
}
}
