#include <sigma/opengl/renderer.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>

namespace sigma {
namespace opengl {
    const resource::identifier renderer::POINT_LIGHT_EFFECT{ "post_process_effect://point_light" };
    const resource::identifier renderer::POINT_LIGHT_STENCIL_EFFECT{ "post_process_effect://point_light_stencil" };
    const resource::identifier renderer::DIRECTIONAL_LIGHT_EFFECT{ "post_process_effect://directional_light" };
    const resource::identifier renderer::VIGNETTE_EFFECT{ "post_process_effect://vignette" };
    const resource::identifier renderer::GAMMA_CONVERSION_EFFECT{ "post_process_effect://gamma_conversion" };

    renderer::renderer(glm::ivec2 size)
        : graphics::renderer(size)
        , default_fbo_(size)
        , gbuffer_(size)
        , textures_(boost::filesystem::current_path() / ".." / "data")
        , shaders_(boost::filesystem::current_path() / ".." / "data")
        , materials_(boost::filesystem::current_path() / ".." / "data", textures_, shaders_)
        , static_meshes_(boost::filesystem::current_path() / ".." / "data", materials_)
        , effects_(boost::filesystem::current_path() / ".." / "data", textures_, shaders_, static_meshes_)
    {
        standard_uniforms_.set_binding_point(shader_technique::STANDARD_UNIFORM_BLOCK_BINDING);

        stencil_clear_effect_ = effects_.get("post_process_effect://stencil_clear");
        texture_blit_effect_ = effects_.get("post_process_effect://texture_blit");

        point_light_effect_ = effects_.get(POINT_LIGHT_EFFECT);

        //TODO were should these go?
        point_light_color_location_ = EFFECT_PTR(point_light_effect_)->get_uniform_location("light.color");
        point_light_position_location_ = EFFECT_PTR(point_light_effect_)->get_uniform_location("light.position");
        point_light_radius_location_ = EFFECT_PTR(point_light_effect_)->get_uniform_location("light.radius");
        point_light_falloff_location_ = EFFECT_PTR(point_light_effect_)->get_uniform_location("light.falloff");
        point_light_intensity_location_ = EFFECT_PTR(point_light_effect_)->get_uniform_location("light.intensity");
        point_light_stencil_effect_ = effects_.get(POINT_LIGHT_STENCIL_EFFECT);

        directional_light_effect_ = effects_.get(DIRECTIONAL_LIGHT_EFFECT);
        // TODO were should these go?
        directional_light_color_location_ = EFFECT_PTR(directional_light_effect_)->get_uniform_location("light.color");
        directional_light_direction_location_ = EFFECT_PTR(directional_light_effect_)->get_uniform_location("light.direction");
        directional_light_intensity_location_ = EFFECT_PTR(directional_light_effect_)->get_uniform_location("light.intensity");

        vignette_effect_ = effects_.get(VIGNETTE_EFFECT);

        gamma_conversion_ = effects_.get(GAMMA_CONVERSION_EFFECT);
    }

    renderer::~renderer()
    {
    }

    graphics::texture_manager& renderer::textures()
    {
        return textures_;
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

    void renderer::geometry_pass(const graphics::view_port& viewport, bool transparent)
    {
        GL_CHECK(glStencilMask(0xFF));
        GL_CHECK(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GL_CHECK(glEnable(GL_STENCIL_TEST));
        GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));

        GL_CHECK(glDisable(GL_BLEND));

        GL_CHECK(glDepthMask(GL_TRUE));
        GL_CHECK(glDepthFunc(GL_LESS));
        GL_CHECK(glEnable(GL_DEPTH_TEST));

        GL_CHECK(glCullFace(GL_BACK));
        GL_CHECK(glEnable(GL_CULL_FACE));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.static_mesh_instances.has(e) && viewport.transforms.has(e)) {
                auto& txform = viewport.transforms.get(e);
                auto mesh = STATIC_MESH_PTR(viewport.static_mesh_instances.get(e));
                auto mat = MATERIAL_PTR(mesh->material());

                if (transparent == mat->is_transparent()) {
                    matrices_.model_matrix = txform.matrix();

                    matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                    matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                    mat->bind(&matrices_, texture_unit::TEXTURE0);
                    mesh->render();
                }
            }
        }
        GL_CHECK(glDepthMask(GL_FALSE));
        GL_CHECK(glStencilMask(0x00));
    }

    void renderer::light_pass(const graphics::view_port& viewport)
    {
        GL_CHECK(glStencilMask(0x00));
        GL_CHECK(glStencilFunc(GL_EQUAL, 1, 0xFF));
        GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
        GL_CHECK(glEnable(GL_STENCIL_TEST));

        GL_CHECK(glBlendEquation(GL_FUNC_ADD));
        GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));
        GL_CHECK(glEnable(GL_BLEND));

        GL_CHECK(glCullFace(GL_FRONT));
        GL_CHECK(glEnable(GL_CULL_FACE));

        GL_CHECK(glDepthFunc(GL_GREATER));
        GL_CHECK(glEnable(GL_DEPTH_TEST));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.point_lights.has(e) && viewport.transforms.has(e)) {
                auto& txform = viewport.transforms.get(e);
                const auto& light = viewport.point_lights.get(e);

                // TODO non uniform scale on point light???
                matrices_.model_matrix = txform.matrix();

                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                point_light_pass(txform, light);
            }
        }

        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_CULL_FACE));

        // TODO directional lights
        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.directional_lights.has(e) && viewport.transforms.has(e)) {
                auto& txform = viewport.transforms.get(e);
                const auto& light = viewport.directional_lights.get(e);

                // TODO scale on directional light???
                matrices_.model_matrix = txform.matrix();

                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                directional_light_pass(txform, light);
            }
        }
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        standard_uniforms data;
        data.projection_matrix = viewport.projection_matrix;
        data.view_matrix = viewport.view_matrix;
        data.view_port_size = viewport.size;
        data.time = 0; // TODO time
        data.z_near = viewport.z_near;
        data.z_far = viewport.z_far;
        standard_uniforms_.set_data(data);

        matrices_.model_matrix = glm::mat4(1);
        matrices_.model_view_matrix = glm::mat4(1);
        matrices_.normal_matrix = glm::mat3(1);

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClearStencil(0));

        // Opaque objects
        gbuffer_.bind_for_geometry_write();
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        geometry_pass(viewport, false);

        gbuffer_.bind_for_geometry_read();
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
        light_pass(viewport);

        // Transparent objects
        gbuffer_.swap_input_image();
        gbuffer_.bind_for_geometry_write();
        geometry_pass(viewport, true);

        gbuffer_.bind_for_geometry_read();
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
        light_pass(viewport);

        gbuffer_.bind_for_geometry_read();
        GL_CHECK(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_CULL_FACE));
        EFFECT_PTR(texture_blit_effect_)->apply(&matrices_);

        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glDisable(GL_STENCIL_TEST));
        GL_CHECK(glDisable(GL_DEPTH_TEST));

        // gbuffer_.swap_input_image();
        // gbuffer_.bind_for_effect_pass(glm::vec4(0, 0, 0, 1));
        // EFFECT_PTR(vignette_effect_)->apply(&matrices_);

        gbuffer_.swap_input_image();
        gbuffer_.bind_for_geometry_read();

        default_fbo_.bind(frame_buffer::target::DRAW);
        GL_CHECK(glClearColor(0, 0, 0, 1));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        EFFECT_PTR(gamma_conversion_)->apply(&matrices_);
    }

    void renderer::point_light_pass(const transform& txform, const graphics::point_light& light)
    {
        auto view_space_position = matrices_.model_view_matrix * glm::vec4(0, 0, 0, 1);

        EFFECT_PTR(point_light_effect_)->bind();
        // TODO this should be abstracted away better
        GL_CHECK(glUniform3fv(point_light_color_location_, 1, glm::value_ptr(light.color)));
        GL_CHECK(glUniform3fv(point_light_position_location_, 1, glm::value_ptr(view_space_position)));
        GL_CHECK(glUniform1f(point_light_radius_location_, std::abs(txform.scale().x))); // TODO non uniform scale on point light???
        GL_CHECK(glUniform1f(point_light_falloff_location_, light.falloff));
        GL_CHECK(glUniform1f(point_light_intensity_location_, light.intensity));
        EFFECT_PTR(point_light_effect_)->apply(&matrices_);
    }

    void renderer::directional_light_pass(const transform& txform, const graphics::directional_light& light)
    {
        auto view_space_direction = matrices_.model_view_matrix * glm::vec4(0, 1, 0, 0);

        EFFECT_PTR(directional_light_effect_)->bind();
        // TODO this should be abstracted away better
        GL_CHECK(glUniform3fv(directional_light_color_location_, 1, glm::value_ptr(light.color)));
        GL_CHECK(glUniform3fv(directional_light_direction_location_, 1, glm::value_ptr(view_space_direction)));
        GL_CHECK(glUniform1f(directional_light_intensity_location_, light.intensity));
        EFFECT_PTR(directional_light_effect_)->apply(&matrices_);
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

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)
