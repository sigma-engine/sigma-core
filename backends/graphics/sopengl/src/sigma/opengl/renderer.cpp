#include <sigma/opengl/renderer.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/scene.hpp>
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
        //standard_uniforms_.set_binding_point(shader_technique::STANDARD_UNIFORM_BLOCK_BINDING);

        point_light_effect_ = effects_.get(POINT_LIGHT_EFFECT);
        point_light_stencil_effect_ = effects_.get(POINT_LIGHT_STENCIL_EFFECT);

        directional_light_effect_ = effects_.get(DIRECTIONAL_LIGHT_EFFECT);

        texture_blit_effect_ = effects_.get("post_process_effect://texture_blit");

        vignette_effect_ = effects_.get(VIGNETTE_EFFECT);
        gamma_conversion_ = effects_.get(GAMMA_CONVERSION_EFFECT);
    }

    renderer::~renderer()
    {
        glDeleteBuffers(1, &directional_light_data_buffer_);
        glDeleteBuffers(1, &point_light_data_buffer_);
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

    std::shared_ptr<graphics::scene> renderer::create_scene()
    {
        return std::make_shared<opengl::scene>();
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
                    render_matrices matrices_;
                    matrices_.model_matrix = txform.matrix();
                    matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                    matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                    mat->bind(texture_unit::TEXTURE0);
                    mat->set_instance_matrices(&standard_uniform_data_, &matrices_);
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

        // Render point lights
        GL_CHECK(glCullFace(GL_FRONT));
        GL_CHECK(glEnable(GL_CULL_FACE));

        GL_CHECK(glDepthFunc(GL_GREATER));
        GL_CHECK(glEnable(GL_DEPTH_TEST));

        render_matrices matrices_;
        matrices_.model_matrix = glm::mat4(1);
        matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
        matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

        EFFECT_PTR(point_light_effect_)->bind();
        EFFECT_PTR(point_light_effect_)->set_instance_matrices(&standard_uniform_data_, &matrices_);

        // TODO this is a hack
        auto point_mesh = STATIC_MESH_PTR(EFFECT_PTR(point_light_effect_)->mesh_);
        GL_CHECK(glBindVertexArray(point_mesh->vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, point_mesh->index_buffer_));
        GL_CHECK(glDrawElementsInstanced(GL_TRIANGLES, point_mesh->index_count_, GL_UNSIGNED_INT, nullptr, internal_point_lights_.size()));

        // Render directional lights
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_CULL_FACE));

        EFFECT_PTR(directional_light_effect_)->bind();
        EFFECT_PTR(directional_light_effect_)->set_instance_matrices(&standard_uniform_data_, &matrices_);

        // TODO this is a hack
        auto directional_mesh = STATIC_MESH_PTR(EFFECT_PTR(directional_light_effect_)->mesh_);
        GL_CHECK(glBindVertexArray(directional_mesh->vertex_array_));
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, directional_mesh->index_buffer_));
        GL_CHECK(glDrawElementsInstanced(GL_TRIANGLES, directional_mesh->index_count_, GL_UNSIGNED_INT, nullptr, internal_directional_lights_.size()));
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        // TODO remove this hack
        if (!point_light_buffer_filled_) {
            point_light_buffer_filled_ = true;

            for (auto e : viewport.entities) { // TODO use a filter here
                if (viewport.point_lights.has(e) && viewport.transforms.has(e)) {
                    auto& txform = viewport.transforms.get(e);
                    const auto& light = viewport.point_lights.get(e);
                    internal_point_lights_.push_back({ glm::vec4(light.color, light.intensity), glm::vec4(txform.position(), txform.scale().x) });
                }
            }

            auto point_mesh = STATIC_MESH_PTR(EFFECT_PTR(point_light_effect_)->mesh_);
            GL_CHECK(glBindVertexArray(point_mesh->vertex_array_));
            GL_CHECK(glGenBuffers(1, &point_light_data_buffer_));
            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, point_light_data_buffer_));
            GL_CHECK(glBufferData(GL_ARRAY_BUFFER, internal_point_lights_.size() * sizeof(internal_point_light), internal_point_lights_.data(), GL_STATIC_DRAW));

            GL_CHECK(glEnableVertexAttribArray(4));
            GL_CHECK(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(internal_point_light), 0));

            GL_CHECK(glEnableVertexAttribArray(5));
            GL_CHECK(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(internal_point_light), reinterpret_cast<const void*>(sizeof(glm::vec4))));

            GL_CHECK(glVertexAttribDivisor(4, 1));
            GL_CHECK(glVertexAttribDivisor(5, 1));
        }

        // TODO remove this hack
        if (!directional_light_buffer_filled_) {
            directional_light_buffer_filled_ = true;

            for (auto e : viewport.entities) { // TODO use a filter here
                if (viewport.directional_lights.has(e) && viewport.transforms.has(e)) {
                    auto& txform = viewport.transforms.get(e);
                    const auto& light = viewport.directional_lights.get(e);
                    auto direction = glm::vec3(txform.matrix() * glm::vec4(0, 1, 0, 0));
                    internal_directional_lights_.push_back({ glm::vec4(light.color, light.intensity), direction });
                }
            }

            auto directional_mesh = STATIC_MESH_PTR(EFFECT_PTR(directional_light_effect_)->mesh_);
            GL_CHECK(glBindVertexArray(directional_mesh->vertex_array_));
            GL_CHECK(glGenBuffers(1, &directional_light_data_buffer_));
            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, directional_light_data_buffer_));
            GL_CHECK(glBufferData(GL_ARRAY_BUFFER, internal_directional_lights_.size() * sizeof(internal_directional_light), internal_directional_lights_.data(), GL_STATIC_DRAW));

            GL_CHECK(glEnableVertexAttribArray(4));
            GL_CHECK(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(internal_directional_light), 0));

            GL_CHECK(glEnableVertexAttribArray(5));
            GL_CHECK(glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(internal_directional_light), reinterpret_cast<const void*>(sizeof(glm::vec4))));

            GL_CHECK(glVertexAttribDivisor(4, 1));
            GL_CHECK(glVertexAttribDivisor(5, 1));
        }

        standard_uniform_data_.projection_matrix = viewport.projection_matrix;
        standard_uniform_data_.view_matrix = viewport.view_matrix;
        standard_uniform_data_.view_port_size = viewport.size;
        standard_uniform_data_.time = 0; // TODO time
        standard_uniform_data_.z_near = viewport.z_near;
        standard_uniform_data_.z_far = viewport.z_far;
        //standard_uniforms_.set_data(standard_uniform_data_);

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClearStencil(0));

        // Opaque objects
        gbuffer_.bind_for_geometry_write();
        GL_CHECK(glDepthMask(GL_TRUE));
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

        // Final composition

        render_matrices matrices_;
        matrices_.model_matrix = glm::mat4(1);
        matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_view_matrix;
        matrices_.normal_matrix = glm::mat3(1);

        gbuffer_.bind_for_geometry_read();
        GL_CHECK(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_CULL_FACE));

        EFFECT_PTR(texture_blit_effect_)->bind();
        EFFECT_PTR(texture_blit_effect_)->set_instance_matrices(&standard_uniform_data_, &matrices_);
        EFFECT_PTR(texture_blit_effect_)->apply();

        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glDisable(GL_STENCIL_TEST));
        GL_CHECK(glDisable(GL_DEPTH_TEST));

        // gbuffer_.swap_input_image();
        // gbuffer_.bind_for_effect_pass(glm::vec4(0, 0, 0, 1));
        // EFFECT_PTR(vignette_effect_)->bind();
        // EFFECT_PTR(vignette_effect_)->set_instance_matrices(&matrices_);
        // EFFECT_PTR(vignette_effect_)->apply();

        gbuffer_.swap_input_image();
        gbuffer_.bind_for_geometry_read();

        default_fbo_.bind(frame_buffer::target::DRAW);
        GL_CHECK(glClearColor(0, 0, 0, 1));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        EFFECT_PTR(gamma_conversion_)->bind();
        EFFECT_PTR(gamma_conversion_)->set_instance_matrices(&standard_uniform_data_, &matrices_);
        EFFECT_PTR(gamma_conversion_)->apply();
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
