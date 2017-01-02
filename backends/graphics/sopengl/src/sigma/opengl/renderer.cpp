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
    const resource::identifier renderer::TEXTURE_BLIT_EFFECT{ "post_process_effect://texture_blit" };

    renderer::renderer(context* ctx, glm::ivec2 size)
        : graphics::renderer(ctx, size)
        , ctx_(ctx)
        , default_fbo_(size)
        , gbuffer_(size)
        , textures_(ctx_->textures())
        , shaders_(ctx_->shaders())
        , materials_(ctx_->materials(), textures_, shaders_)
        , static_meshes_(ctx_->static_meshes(), materials_)
        , effects_(ctx_->effects(), textures_, shaders_, static_meshes_)
    {
        point_light_effect_ = effects_.get(POINT_LIGHT_EFFECT);
        // TODO were should these go?
        point_light_color_location_ = point_light_effect_->get_uniform_location("light.color");
        point_light_position_location_ = point_light_effect_->get_uniform_location("light.position");
        point_light_radius_location_ = point_light_effect_->get_uniform_location("light.radius");
        point_light_falloff_location_ = point_light_effect_->get_uniform_location("light.falloff");
        point_light_intensity_location_ = point_light_effect_->get_uniform_location("light.intensity");
        point_light_stencil_effect_ = effects_.get(POINT_LIGHT_STENCIL_EFFECT);

        directional_light_effect_ = effects_.get(DIRECTIONAL_LIGHT_EFFECT);
        // TODO were should these go?
        directional_light_color_location_ = directional_light_effect_->get_uniform_location("light.color");
        directional_light_direction_location_ = directional_light_effect_->get_uniform_location("light.direction");
        directional_light_intensity_location_ = directional_light_effect_->get_uniform_location("light.intensity");

        vignette_effect_ = effects_.get(VIGNETTE_EFFECT);

        fullscreen_blit_ = effects_.get(TEXTURE_BLIT_EFFECT);
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
        gbuffer_.bind_for_geometry_pass();

        GL_CHECK(glDisable(GL_BLEND));

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glDepthMask(GL_TRUE));

        GL_CHECK(glEnable(GL_CULL_FACE));
        GL_CHECK(glCullFace(GL_BACK));

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.transforms.has(e)) {
                auto& txform = viewport.transforms.get(e); // TODO const
                if (viewport.static_mesh_instances.has(e)) {
                    auto mesh = static_meshes_.get(viewport.static_mesh_instances.get(e));

                    matrices_.model_matrix = txform.matrix();

                    matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                    matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                    mesh->render(&matrices_, texture_unit::TEXTURE0);
                }
            }
        }

        GL_CHECK(glDepthMask(GL_FALSE));
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        matrices_.projection_matrix = viewport.projection_matrix;
        matrices_.view_matrix = viewport.view_matrix;
        matrices_.z_near = viewport.z_near;
        matrices_.z_far = viewport.z_far;
        matrices_.size = viewport.size;
        matrices_.model_matrix = glm::mat4(1);
        matrices_.model_view_matrix = glm::mat4(1);
        matrices_.normal_matrix = glm::mat3(1);

        default_fbo_.bind();
        GL_CHECK(glClearColor(0, 0, 0, 1));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        gbuffer_.clear_input_image(glm::vec4(0, 0, 0, 1));

        geometry_pass(viewport);

        GL_CHECK(glEnable(GL_BLEND));
        GL_CHECK(glBlendEquation(GL_FUNC_ADD));
        GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.transforms.has(e) && viewport.point_lights.has(e)) {
                auto& txform = viewport.transforms.get(e); // TODO const
                const auto& light = viewport.point_lights.get(e);

                // TODO non uniform scale on point light???
                matrices_.model_matrix = txform.matrix();

                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                //point_light_stencil_pass(txform, light);
                point_light_pass(txform, light);
            }
        }

        // TODO directional lights
        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.transforms.has(e) && viewport.directional_lights.has(e)) {
                auto& txform = viewport.transforms.get(e); // TODO const
                const auto& light = viewport.directional_lights.get(e);

                // TODO scale on directional light???
                matrices_.model_matrix = txform.matrix();

                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices_.model_view_matrix)));

                directional_light_pass(txform, light);
            }
        }

        /*GL_CHECK(glDisable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_STENCIL_TEST));
        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glDisable(GL_CULL_FACE));*/
        // gbuffer_.swap_input_image();
        // gbuffer_.bind_for_effect_pass();
        // vignette_effect_->apply(&matrices_);

        gbuffer_.swap_input_image();
        gbuffer_.bind_for_effect_pass();
        default_fbo_.bind(frame_buffer::target::DRAW);
        fullscreen_blit_->apply(&matrices_);
    }

    void renderer::point_light_pass(const transform& txform, const graphics::point_light& light)
    {
        // TODO this should be pre calculated
        //glm::vec3 cpos(glm::inverse(matrices_.view_matrix) * glm::vec4(0, 0, 0, 1));

        GL_CHECK(glEnable(GL_DEPTH_TEST));

        auto view_space_position = matrices_.model_view_matrix * glm::vec4(0, 0, 0, 1);
        if (glm::length(view_space_position) <= 1.1 * txform.scale().x) {
            GL_CHECK(glCullFace(GL_FRONT));
            GL_CHECK(glDepthFunc(GL_GREATER));
        } else {
            gbuffer_.bind_for_stencil_pass();

            // http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
            GL_CHECK(glEnable(GL_STENCIL_TEST));
            GL_CHECK(glClearStencil(4));
            GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));

            GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
            GL_CHECK(glStencilFunc(GL_GEQUAL, 6, 0xFF));

            GL_CHECK(glDepthFunc(GL_GEQUAL));
            GL_CHECK(glCullFace(GL_FRONT));

            point_light_stencil_effect_->apply(&matrices_);

            GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
            GL_CHECK(glStencilFunc(GL_LEQUAL, 5, 0xFF));

            GL_CHECK(glDepthFunc(GL_LEQUAL));
            GL_CHECK(glCullFace(GL_BACK));
        }

        gbuffer_.bind_for_effect_pass();
        point_light_effect_->bind();

        // TODO this should be abstracted away better
        GL_CHECK(glUniform3fv(point_light_color_location_, 1, glm::value_ptr(light.color)));
        GL_CHECK(glUniform3fv(point_light_position_location_, 1, glm::value_ptr(view_space_position)));
        GL_CHECK(glUniform1f(point_light_radius_location_, std::abs(txform.scale().x))); // TODO non uniform scale on point light???
        GL_CHECK(glUniform1f(point_light_falloff_location_, light.falloff));
        GL_CHECK(glUniform1f(point_light_intensity_location_, light.intensity));

        point_light_effect_->apply(&matrices_);

        GL_CHECK(glDepthFunc(GL_LESS));
        GL_CHECK(glDisable(GL_STENCIL_TEST));
        GL_CHECK(glCullFace(GL_BACK));
    }

    void renderer::directional_light_pass(const transform& txform, const graphics::directional_light& light)
    {
        GL_CHECK(glDisable(GL_DEPTH_TEST));

        gbuffer_.bind_for_effect_pass();

        auto view_space_direction = matrices_.model_view_matrix * glm::vec4(0, 1, 0, 0);

        directional_light_effect_->bind();
        GL_CHECK(glUniform3fv(directional_light_color_location_, 1, glm::value_ptr(light.color)));
        GL_CHECK(glUniform3fv(directional_light_direction_location_, 1, glm::value_ptr(view_space_direction)));
        GL_CHECK(glUniform1f(directional_light_intensity_location_, light.intensity));

        directional_light_effect_->apply(&matrices_);
    }
}
}

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)
