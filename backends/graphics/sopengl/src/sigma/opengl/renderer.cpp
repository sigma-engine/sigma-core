#include <sigma/opengl/renderer.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

namespace sigma {
namespace opengl {
    const resource::identifier renderer::TEXTURE_BLIT_EFFECT{ "post_process_effect://texture_blit" };
    const resource::identifier renderer::DIRECTIONAL_LIGHT_EFFECT{ "post_process_effect://directional_light" };
    const resource::identifier renderer::POINT_LIGHT_EFFECT{ "post_process_effect://point_light" };

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
        fullscreen_blit_ = effects_.get_interal(TEXTURE_BLIT_EFFECT);
        directional_light_effect_ = effects_.get_interal(DIRECTIONAL_LIGHT_EFFECT);
        point_light_effect_ = effects_.get_interal(POINT_LIGHT_EFFECT);
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

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glDepthMask(GL_TRUE));

        GL_CHECK(glEnable(GL_CULL_FACE));
        GL_CHECK(glCullFace(GL_BACK));

        GL_CHECK(glDisable(GL_BLEND));

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.transforms.has(e)) {
                const auto& txform = viewport.transforms.get(e);
                if (viewport.static_mesh_instances.has(e)) {
                    auto mesh = static_meshes_.get(viewport.static_mesh_instances.get(e));

                    // TODO move this into an ecs
                    matrices_.model_matrix = glm::mat4(1);
                    matrices_.model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(matrices_.model_matrix, txform.scale), txform.position);
                    matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                    matrices_.normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrices_.model_view_matrix)));

                    mesh->render(&matrices_, 0);
                }
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

        gbuffer_.bind_textures();

        //GL_CHECK(glDisable(GL_BLEND));

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.transforms.has(e) && viewport.point_lights.has(e)) {
                const auto& txform = viewport.transforms.get(e);
                const auto& light = viewport.point_lights.get(e);

                // TODO move this into an ecs
				auto radius = glm::vec3(glm::length(txform.scale));
                matrices_.model_matrix = glm::mat4(1);
                matrices_.model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(matrices_.model_matrix, radius), txform.position);
                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrices_.model_view_matrix)));

                auto color_loc = point_light_effect_->get_uniform_location("light.color");
				auto position_loc = point_light_effect_->get_uniform_location("light.position");
				auto radius_loc = point_light_effect_->get_uniform_location("light.radius");
				auto falloff_loc = point_light_effect_->get_uniform_location("light.falloff");
				auto intensity_loc = point_light_effect_->get_uniform_location("light.intensity");

				GL_CHECK(glUniform3fv(color_loc, 1, glm::value_ptr(light.color)));
                GL_CHECK(glUniform3fv(position_loc, 1, glm::value_ptr(txform.position)));
                GL_CHECK(glUniform1f(radius_loc, radius.x));
				GL_CHECK(glUniform1f(falloff_loc, light.falloff));
				GL_CHECK(glUniform1f(intensity_loc, light.intensity));
                point_light_effect_->apply(&matrices_);
            }
            /*if (viewport.transforms.has(e) && viewport.directional_lights.has(e)) {
                const auto& txform = viewport.transforms.get(e);
                const auto& light = viewport.directional_lights.get(e);

                // TODO move this into an ecs
                matrices_.model_matrix = glm::mat4(1);
                matrices_.model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(matrices_.model_matrix, txform.scale), txform.position);
                matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_matrix;
                matrices_.normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrices_.model_view_matrix)));

                glm::vec3 direction{ matrices_.model_matrix * glm::vec4(0, 0, 1, 0) };

                auto color_loc = directional_light_effect_->get_uniform_location("color");
                auto direction_loc = directional_light_effect_->get_uniform_location("direction");
                GL_CHECK(glUniform3fv(color_loc, 1, glm::value_ptr(light.color)));
                GL_CHECK(glUniform3fv(direction_loc, 1, glm::value_ptr(direction)));
                directional_light_effect_->apply(&matrices_);
            }*/
        }

        //fullscreen_blit_->apply(&matrices_, 1);

        end_light_pass();
    }

    void renderer::end_light_pass()
    {
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        matrices_.projection_matrix = viewport.projection_matrix;
        matrices_.view_matrix = viewport.view_matrix;
        matrices_.size = viewport.size;

        geometry_pass(viewport);
        light_pass(viewport);
    }
}
}

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)
