#include <sigma/graphics/opengl/renderer.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

#include <glad/glad.h>

#include <stdexcept>

namespace sigma {
namespace opengl {
    renderer::renderer(glm::ivec2 size,
        resource::cache<graphics::texture>& texture_cache,
        resource::cache<graphics::cubemap>& cubemap_cache,
        resource::cache<graphics::shader>& shader_cache,
        resource::cache<graphics::material>& material_cache,
        resource::cache<graphics::static_mesh>& static_mesh_cache,
        resource::cache<graphics::post_process_effect>& effect_cache)
        : graphics::renderer(size)
        , size_(size.x, size.y)
        , loader_status_(gladLoadGL())
        , default_fbo_(size)
        , gbuffer_(size)
        , sbuffer_({ 1024, 1024 }, 3)
        , start_time_(std::chrono::high_resolution_clock::now())
        , textures_(texture_cache)
        , cubemaps_(cubemap_cache)
        , shaders_(shader_cache)
        , materials_(textures_, cubemaps_, shaders_, material_cache)
        , static_meshes_(materials_, static_mesh_cache)
        , effects_(textures_, cubemaps_, shaders_, static_meshes_, effect_cache)
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        debug_renderer_.windowWidth = size.x;
        debug_renderer_.windowHeight = size.y;
        dd::initialize(&debug_renderer_);

        image_based_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/image_based");
        point_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/point");
        directional_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/directional");
        spot_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/spot");
        texture_blit_effect_ = effects_.get("post_process_effect://pbr/deffered/texture_blit");

        shadow_material_ = materials_.get("material://shadow");

        vignette_effect_ = effects_.get("post_process_effect://vignette");
        gamma_conversion_ = effects_.get("post_process_effect://pbr/deffered/gamma_conversion");

        standard_uniform_buffer_.set_binding_point(0);
    }

    renderer::~renderer()
    {
        dd::shutdown();
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
