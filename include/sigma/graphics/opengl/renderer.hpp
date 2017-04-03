#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/graphics/renderer.hpp>

#include <sigma/config.hpp>
#include <sigma/entity.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/frame_buffer.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/material.hpp>
#include <sigma/graphics/opengl/post_process_effect.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/shadow_buffer.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/uniform_buffer.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>

#include <limits>

namespace sigma {

namespace opengl {
    class SIGMA_API renderer : public graphics::renderer {
    public:
        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual graphics::texture_manager& textures() override;

        virtual graphics::cubemap_manager& cubemaps() override;

        virtual graphics::shader_manager& shaders() override;

        virtual graphics::material_manager& materials() override;

        virtual graphics::static_mesh_manager& static_meshes() override;

        virtual graphics::post_process_effect_manager& effects() override;

        virtual void resize(glm::uvec2 size) override;

        template <class World>
        void render(const graphics::view_port& viewport, World& world)
        {
            setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
            standard_uniform_data_.view_port_size = viewport.size;
            standard_uniform_data_.time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - start_time_).count();
            standard_uniform_data_.z_near = viewport.view_frustum.z_near();
            standard_uniform_data_.z_far = viewport.view_frustum.z_far();
            //standard_uniforms_.set_data(standard_uniform_data_);

            // Begin rendering
            GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GL_CHECK(glClearStencil(0));

            // Opaque objects
            gbuffer_.bind_for_geometry_write();
            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            geometry_pass(viewport, world, false);

            light_pass(viewport, world);

            // // Transparent objects
            // gbuffer_.swap_input_image();
            // gbuffer_.bind_for_geometry_write();
            // geometry_pass(viewport, true);
            //
            // // TODO is energy conserved here??
            // gbuffer_.bind_for_geometry_read();
            // GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
            // light_pass(viewport);
            //
            // // Final transparancy composition
            // gbuffer_.bind_for_geometry_read();
            // GL_CHECK(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
            // GL_CHECK(glDisable(GL_DEPTH_TEST));
            // GL_CHECK(glDisable(GL_CULL_FACE));
            //
            // EFFECT_PTR(texture_blit_effect_)->bind();
            // EFFECT_PTR(texture_blit_effect_)->set_standard_uniforms(&standard_uniform_data_);
            // EFFECT_PTR(texture_blit_effect_)->apply();

            // Render final effects
            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));

            // gbuffer_.swap_input_image();
            // gbuffer_.bind_for_geometry_read();
            // EFFECT_PTR(vignette_effect_)->bind();
            // EFFECT_PTR(vignette_effect_)->set_standard_uniforms(&standard_uniform_data_);
            // EFFECT_PTR(vignette_effect_)->apply();

            gbuffer_.swap_input_image();
            gbuffer_.bind_for_geometry_read();

            default_fbo_.bind(frame_buffer::target::DRAW);
            GL_CHECK(glClearColor(0, 0, 0, 1));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

            EFFECT_CONST_PTR(gamma_conversion_)->bind();
            EFFECT_CONST_PTR(gamma_conversion_)->set_standard_uniforms(&standard_uniform_data_);
            EFFECT_CONST_PTR(gamma_conversion_)->apply();
        }

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        int loader_status_;
        default_frame_buffer default_fbo_;
        geometry_buffer gbuffer_;
        shadow_buffer sbuffer_;

        std::chrono::high_resolution_clock::time_point start_time_;
        standard_uniforms standard_uniform_data_;
        //uniform_buffer<standard_uniforms> standard_uniforms_;

        opengl::shader_manager shaders_;
        opengl::texture_manager textures_;
        opengl::cubemap_manager cubemaps_;
        opengl::material_manager materials_;
        opengl::static_mesh_manager static_meshes_;
        opengl::post_process_effect_manager effects_;

        resource::handle<graphics::post_process_effect> image_based_light_effect_;
        resource::handle<graphics::post_process_effect> point_light_effect_;
        resource::handle<graphics::post_process_effect> directional_light_effect_;
        resource::handle<graphics::post_process_effect> spot_light_effect_;
        resource::handle<graphics::post_process_effect> texture_blit_effect_;

        resource::handle<graphics::material> shadow_material_;

        resource::handle<graphics::post_process_effect> gamma_conversion_;

        resource::handle<graphics::post_process_effect> vignette_effect_;

        void setup_view_projection(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
        {
            standard_uniform_data_.projection_matrix = projection_matrix;
            standard_uniform_data_.view_matrix = view_matrix;
            standard_uniform_data_.inverse_projection_matrix = glm::inverse(projection_matrix);
            standard_uniform_data_.inverse_view_matrix = glm::inverse(view_matrix);
            standard_uniform_data_.projection_view_matrix = projection_matrix * view_matrix;
            standard_uniform_data_.inverse_projection_view_matrix = glm::inverse(standard_uniform_data_.projection_view_matrix);
            standard_uniform_data_.eye_position = glm::vec3(standard_uniform_data_.inverse_view_matrix * glm::vec4(0, 0, 0, 1));
        }

        template <class World>
        void geometry_pass(const graphics::view_port& viewport, World& world, bool transparent)
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

            world.template for_each<transform, graphics::static_mesh_instance>([&](entity e, const transform& txform, graphics::static_mesh_instance& mesh_instance) {
                auto mesh = STATIC_MESH_PTR(mesh_instance.mesh);
                instance_matrices matrices;
                matrices.model_matrix = txform.matrix;
                matrices.model_view_matrix = standard_uniform_data_.view_matrix * matrices.model_matrix;
                matrices.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices.model_matrix))); //glm::transpose(glm::inverse(glm::mat3(matrices.model_view_matrix)));

                for (unsigned int i = 0; i < mesh->material_count(); ++i) {
                    auto mat = MATERIAL_PTR(mesh->material(i));

                    auto mit = mesh_instance.materials.find(i);
                    if (mit != mesh_instance.materials.end()) {
                        mat = MATERIAL_PTR(mit->second);
                    }

                    if (mat->is_transparent() == transparent) {
                        mat->bind(geometry_buffer::NEXT_FREE_TEXTURE_UINT);
                        mat->set_standard_uniforms(&standard_uniform_data_);
                        mat->set_instance_matrices(&matrices);
                        mesh->render(i);
                    }
                }
            });

            GL_CHECK(glDepthMask(GL_FALSE));
            GL_CHECK(glStencilMask(0x00));
        }

        template <class World>
        void light_pass(const graphics::view_port& viewport, World& world)
        {
            // GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

            // TODO:perf look into passing all analytical lights(directional,point,spot) into one shader
            // instead of rendering the geometry for each. This not only would reduce the number of polygons
            // drawn but more inportantly would mean that the gbuffer would only have to be sampled once per
            // screen pixel.

            // Render Image based lighting
            image_based_light_pass(viewport, world);

            // Render directional lights
            directional_light_pass(viewport, world);

            // Render point lights
            //point_light_pass(viewport, world);

            // TODO Render spot lights
            spot_light_pass(viewport, world);
        }

        template <class World>
        void image_based_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO IBL breaks energy conservation of the environment map
            // when transparancies are rendered.

            setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
            gbuffer_.bind_for_geometry_read();

            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));

            EFFECT_CONST_PTR(image_based_light_effect_)->bind();
            EFFECT_CONST_PTR(image_based_light_effect_)->set_standard_uniforms(&standard_uniform_data_);
            EFFECT_CONST_PTR(image_based_light_effect_)->apply();
        }

        void analytical_light_setup()
        {
            GL_CHECK(glStencilMask(0x00));
            GL_CHECK(glStencilFunc(GL_EQUAL, 1, 0xFF));
            GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
            GL_CHECK(glEnable(GL_STENCIL_TEST));

            GL_CHECK(glBlendEquation(GL_FUNC_ADD));
            GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));
            GL_CHECK(glEnable(GL_BLEND));
        }

        template <class World>
        void directional_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO:perf we can use one fullscreen quad to render all of the directional lights and save on gbuffer lookups.
            setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
            auto inverse_projection_view_matrix = standard_uniform_data_.inverse_projection_view_matrix;
            auto epos = standard_uniform_data_.eye_position;

            glm::vec4 corners[] = {
                glm::vec4{ -1, -1, -1, 1 },
                glm::vec4{ -1, 1, -1, 1 },
                glm::vec4{ 1, 1, -1, 1 },
                glm::vec4{ 1, -1, -1, 1 },
                glm::vec4{ -1, -1, 1, 1 },
                glm::vec4{ -1, 1, 1, 1 },
                glm::vec4{ 1, 1, 1, 1 },
                glm::vec4{ 1, -1, 1, 1 }
            };

            for (auto& c : corners) {
                c = inverse_projection_view_matrix * c;
                c /= c.w;
            }

            float longest_diagonal = glm::length(glm::vec3(corners[6] - corners[0])) / 2.0f;
            auto projection = glm::ortho(-longest_diagonal, longest_diagonal, -longest_diagonal, longest_diagonal, 0.0f, 2.0f * longest_diagonal);

            world.template for_each<transform, graphics::directional_light>([&](entity e, const transform& txform, const graphics::directional_light& light) {
                auto light_direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));

                auto light_center = epos + (longest_diagonal * light_direction);
                light_center = glm::round(light_center * float(sbuffer_.size().x)) / float(sbuffer_.size().x);
                auto view = glm::lookAt(light_center, light_center - light_direction, glm::vec3(0, 1, 0));

                standard_uniform_data_.light_projection_view_matrix = projection * view;
                setup_view_projection(view, projection);

                render_to_shadow_map(world, light.cast_shadows);

                setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
                gbuffer_.bind_for_geometry_read();

                analytical_light_setup();

                GL_CHECK(glDisable(GL_DEPTH_TEST));
                GL_CHECK(glDisable(GL_CULL_FACE));

                EFFECT_PTR(directional_light_effect_)->bind();
                EFFECT_PTR(directional_light_effect_)->set_standard_uniforms(&standard_uniform_data_);

                EFFECT_PTR(directional_light_effect_)->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                EFFECT_PTR(directional_light_effect_)->set_uniform("direction", light_direction);
                EFFECT_PTR(directional_light_effect_)->set_uniform("center", light_center);
                EFFECT_PTR(directional_light_effect_)->bind(); // TODO update uniforms
                EFFECT_PTR(directional_light_effect_)->apply();
            });
        }

        template <class World>
        void point_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO:perf re-incorporate the stencil test that limits the pixels being light see
            // http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
            // TODO:perf make sure that using instanceing is a win here, if it is not go back to
            // standard drawing methods.
            // TODO:perf look into using a quad for rendering point lights.
            // TODO:perf look into using a fullscreen quad for all point lights and have just one pass
            // that does all point light lighting at once.

            setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
            gbuffer_.bind_for_geometry_read();

            analytical_light_setup();

            GL_CHECK(glDepthFunc(GL_GREATER));
            GL_CHECK(glEnable(GL_DEPTH_TEST));

            GL_CHECK(glCullFace(GL_FRONT));
            GL_CHECK(glEnable(GL_CULL_FACE));

            EFFECT_CONST_PTR(point_light_effect_)->bind();
            EFFECT_CONST_PTR(point_light_effect_)->set_standard_uniforms(&standard_uniform_data_);

            world.template for_each<transform, graphics::point_light>([&](entity e, const transform& txform, const graphics::point_light& light) {
                EFFECT_PTR(point_light_effect_)->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                EFFECT_PTR(point_light_effect_)->set_uniform("position_radius", glm::vec4(txform.position, txform.scale.x));
                EFFECT_PTR(point_light_effect_)->bind(); // TODO update uniforms
                EFFECT_PTR(point_light_effect_)->apply();
            });
        }

        template <class World>
        void spot_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO:perf render cones for spot lights to limit there effects.

            world.template for_each<transform, graphics::spot_light>([&](entity e, transform& txform, const graphics::spot_light& light) {
                auto light_direction = glm::normalize(glm::vec3(txform.matrix * glm::vec4(0, 1, 0, 0)));
                auto projection = glm::perspective(light.cutoff * 2.0f, 1.0f, 0.01f, 50.0f);
                auto view = glm::lookAt(txform.position, txform.position - light_direction, glm::vec3(0, 1, 0));
                standard_uniform_data_.light_projection_view_matrix = projection * view;
                setup_view_projection(view, projection);

                render_to_shadow_map(world, light.cast_shadows);

                setup_view_projection(viewport.view_frustum.view(), viewport.view_frustum.projection());
                gbuffer_.bind_for_geometry_read();

                analytical_light_setup();

                GL_CHECK(glDisable(GL_DEPTH_TEST));
                GL_CHECK(glDisable(GL_CULL_FACE));

                EFFECT_CONST_PTR(spot_light_effect_)->bind();
                EFFECT_CONST_PTR(spot_light_effect_)->set_standard_uniforms(&standard_uniform_data_);

                EFFECT_PTR(spot_light_effect_)->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                EFFECT_PTR(spot_light_effect_)->set_uniform("position", txform.position);
                EFFECT_PTR(spot_light_effect_)->set_uniform("direction", light_direction);
                EFFECT_PTR(spot_light_effect_)->set_uniform("cutoff", std::cos(light.cutoff));
                EFFECT_PTR(spot_light_effect_)->bind(); // TODO update uniforms
                EFFECT_PTR(spot_light_effect_)->apply();
            });
        }

        template <class World>
        void render_to_shadow_map(World& world, bool cast_shadows = true)
        {
            sbuffer_.bind_for_shadow_write();

            GL_CHECK(glDisable(GL_BLEND));

            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glDepthFunc(GL_LESS));
            GL_CHECK(glEnable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_STENCIL_TEST));

            GL_CHECK(glCullFace(GL_BACK));
            GL_CHECK(glEnable(GL_CULL_FACE));

            GL_CHECK(glClearColor(-1.0f, -1.0f, 0.0f, 0.0f));
            GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

            MATERIAL_PTR(shadow_material_)->bind(geometry_buffer::NEXT_FREE_TEXTURE_UINT);
            MATERIAL_PTR(shadow_material_)->set_standard_uniforms(&standard_uniform_data_);

            if (cast_shadows) {
                world.template for_each<transform, graphics::static_mesh_instance>([&](entity e, const transform& txform, graphics::static_mesh_instance& mesh_instance) {
                    if (mesh_instance.cast_shadows) {
                        auto mesh = STATIC_MESH_PTR(mesh_instance.mesh);
                        instance_matrices matrices;
                        matrices.model_matrix = txform.matrix;
                        matrices.model_view_matrix = standard_uniform_data_.view_matrix * matrices.model_matrix;

                        MATERIAL_PTR(shadow_material_)->set_instance_matrices(&matrices);

                        mesh->render_all();
                    }
                });
            }

            GL_CHECK(glDepthMask(GL_FALSE));
            GL_CHECK(glStencilMask(0x00));
            sbuffer_.bind_for_shadow_read(geometry_buffer::SHADOW_MAP_TEXTURE_UINT);
        }

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
