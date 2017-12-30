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
    void calculate_cascade_frustums(const frustum& view_frustum, frustum* cascade_frustums, int count)
    {
        // TODO move this into viewport or
        // what ever viewport gets refactored into.
        float cascade_splits[count + 1];
        const float lambda = 0.7f;
        const float n = view_frustum.z_near();
        const float f = view_frustum.z_far();
        int m = count;
        for (int i = 0; i < m; ++i) {
            float fi = float(i) / m;
            float Ci_log = n * std::pow(f / n, fi);
            float Ci_uni = n + (f - n) * fi;
            cascade_splits[i] = lambda * Ci_log + (1 - lambda) * Ci_uni;
        }
        cascade_splits[count] = f;

        for (int i = 0; i < count; ++i) {
            cascade_frustums[i] = frustum{
                view_frustum.fovy(), view_frustum.aspect(),
                cascade_splits[i], cascade_splits[i + 1],
                view_frustum.view()
            };
        }
    }

    renderer::renderer(glm::ivec2 size, graphics::renderer::context_view_type ctx)
        : graphics::renderer(size, ctx)
        , loader_status_(gladLoadGL())
        , size_(size.x, size.y)
        , default_fbo_(size_)
        , gbuffer_(size_)
        , sbuffer_({ 1024, 1024 }, 3)
        , start_time_(std::chrono::high_resolution_clock::now())
        , textures_(ctx.get_cache<graphics::texture>())
        , cubemaps_(ctx.get_cache<graphics::texture>(), ctx.get_cache<graphics::cubemap>())
        , shaders_(ctx.get_cache<graphics::shader>())
        , techniques_(shaders_, ctx.get_cache<graphics::technique>())
        , materials_(techniques_, ctx.get_cache<graphics::material>())
        , static_meshes_(materials_, ctx.get_cache<graphics::static_mesh>())
        , effects_(techniques_, static_meshes_, ctx.get_cache<graphics::post_process_effect>())
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        debug_renderer_.windowWidth = size.x;
        debug_renderer_.windowHeight = size.y;
        dd::initialize(&debug_renderer_);

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

    void renderer::begin_effect(opengl::post_process_effect* effect)
    {
        auto tech = TECHNIQUE_PTR(techniques_, effect->data.technique_id);
        tech->bind(textures_, cubemaps_, effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
    }

    void renderer::end_effect(opengl::post_process_effect* effect)
    {
        STATIC_MESH_PTR(static_meshes_, effect->data.mesh)->render_all();
    }

    void renderer::setup_view_projection(const glm::vec2& viewport_size, float fovy, float z_near, float z_far, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
    {
        standard_.projection_matrix = projection_matrix;
        standard_.inverse_projection_matrix = glm::inverse(projection_matrix);
        standard_.view_matrix = view_matrix;
        standard_.inverse_view_matrix = glm::inverse(view_matrix);
        standard_.projection_view_matrix = projection_matrix * view_matrix;
        standard_.inverse_projection_view_matrix = glm::inverse(standard_.projection_view_matrix);
        standard_.view_port_size = viewport_size;
        standard_.eye_position = standard_.inverse_view_matrix * glm::vec4(0, 0, 0, 1);
        standard_.time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - start_time_).count();
        standard_.fovy = fovy;
        standard_.z_near = z_near;
        standard_.z_far = z_far;
        standard_uniform_buffer_.set_data(standard_);
    }

    void renderer::image_based_light_pass(const graphics::view_port& viewport)
    {
        setup_view_projection(size_,
            viewport.view_frustum.fovy(),
            viewport.view_frustum.z_near(),
            viewport.view_frustum.z_far(),
            viewport.view_frustum.view(),
            viewport.view_frustum.projection());
        gbuffer_.bind_for_geometry_read();

        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_CULL_FACE));

        auto image_based_light_effect = EFFECT_PTR(effects_, settings_.image_based_light_effect);
        auto image_based_light_tech = TECHNIQUE_PTR(techniques_, image_based_light_effect->data.technique_id);
        image_based_light_tech->bind(textures_, cubemaps_, image_based_light_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        STATIC_MESH_PTR(static_meshes_, image_based_light_effect->data.mesh)->render_all();
    }

    void renderer::analytical_light_setup()
    {
        GL_CHECK(glBlendEquation(GL_FUNC_ADD));
        GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));
        GL_CHECK(glEnable(GL_BLEND));
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
