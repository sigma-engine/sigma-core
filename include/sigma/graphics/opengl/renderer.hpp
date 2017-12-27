#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/renderer.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/cubemap.hpp>
// #include <sigma/graphics/opengl/debug_draw_renderer.hpp>
#include <sigma/graphics/opengl/frame_buffer.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/material.hpp>
#include <sigma/graphics/opengl/post_process_effect.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/shadow_buffer.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/technique.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/uniform_buffer.hpp>
#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/shadow_block.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/standard_block.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/cache.hpp>
#include <sigma/transform.hpp>
#include <sigma/world.hpp>

#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <chrono>

namespace sigma {

namespace opengl {
    static inline void calculate_cascade_frustums(const frustum& view_frustum, frustum* cascade_frustums, int count)
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
    class SIGMA_API renderer : public graphics::renderer {
    public:
        bool save_frustums = false;
        std::vector<std::pair<glm::vec3, glm::mat4>> debug_frustums;

        renderer(glm::ivec2 size,
            resource::cache<graphics::texture>& texture_cache,
            resource::cache<graphics::cubemap>& cubemap_cache,
            resource::cache<graphics::shader>& shader_cahce,
            resource::cache<graphics::technique>& technique_cahce,
            resource::cache<graphics::material>& material_cache,
            resource::cache<graphics::static_mesh>& static_mesh_cache,
            resource::cache<graphics::post_process_effect>& effect_cache);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) override;

        template <class World>
        void render(const graphics::view_port& viewport, World& world)
        {
            // debug_renderer_.mvpMatrix = viewport.view_frustum.projection_view();
            // for (const auto& f : debug_frustums)
            //     dd::frustum(glm::value_ptr(f.second), glm::value_ptr(f.first));

            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            // Begin rendering
            GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GL_CHECK(glClearStencil(0));

            // Opaque objects
            gbuffer_.bind_for_geometry_write();
            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            geometry_pass(viewport, world, false);

            light_pass(viewport, world);

            gbuffer_.bind_for_geometry_read();
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
            // dd::flush(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time_).count());

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
            // EFFECT_PTR(texture_blit_effect_)->apply();

            // Render final effects
            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));

            // gbuffer_.swap_input_image();
            // gbuffer_.bind_for_geometry_read();
            // EFFECT_PTR(vignette_effect_)->bind();
            // EFFECT_PTR(vignette_effect_)->apply();

            gbuffer_.swap_input_image();
            gbuffer_.bind_for_geometry_read();

            default_fbo_.bind(frame_buffer::target::DRAW);
            GL_CHECK(glClearColor(0, 0, 0, 1));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

            auto gamma_conversion_effect = EFFECT_PTR(effects_, settings_.gamma_conversion);
            auto gamma_conversion_tech = TECHNIQUE_PTR(techniques_, gamma_conversion_effect->data.technique_id);
            gamma_conversion_tech->bind(textures_, cubemaps_, gamma_conversion_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
            STATIC_MESH_PTR(static_meshes_, gamma_conversion_effect->data.mesh)->render_all();
        }

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        int loader_status_;
        glm::vec2 size_;
        default_frame_buffer default_fbo_;
        geometry_buffer gbuffer_;
        shadow_buffer sbuffer_;

        std::chrono::high_resolution_clock::time_point start_time_;
        graphics::standard_block standard_;
        uniform_buffer<graphics::standard_block> standard_uniform_buffer_;

        graphics::shadow_block shadow_;
        uniform_buffer<graphics::shadow_block> shadow_uniform_buffer_;

        opengl::texture_manager textures_;
        opengl::cubemap_manager cubemaps_;
        opengl::shader_manager shaders_;
        opengl::technique_manager techniques_;
        opengl::material_manager materials_;
        opengl::static_mesh_manager static_meshes_;
        opengl::post_process_effect_manager effects_;

        // debug_draw_renderer debug_renderer_;

        void begin_effect(opengl::post_process_effect* effect)
        {
            auto tech = TECHNIQUE_PTR(techniques_, effect->data.technique_id);
            tech->bind(textures_, cubemaps_, effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        }

        void end_effect(opengl::post_process_effect* effect)
        {
            STATIC_MESH_PTR(static_meshes_, effect->data.mesh)->render_all();
        }

        void setup_view_projection(const glm::vec2& viewport_size, float fovy, float z_near, float z_far, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
        {
            standard_.projection_matrix = projection_matrix;
            standard_.inverse_projection_matrix = glm::inverse(projection_matrix);
            standard_.view_matrix = view_matrix;
            standard_.inverse_view_matrix = glm::inverse(view_matrix);
            standard_.projection_view_matrix = projection_matrix * view_matrix;
            standard_.inverse_projection_view_matrix = glm::inverse(standard_.projection_view_matrix);
            standard_.view_port_size = viewport_size;
            standard_.eye_position = glm::vec3(standard_.inverse_view_matrix * glm::vec4(0, 0, 0, 1));
            standard_.time = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - start_time_).count();
            standard_.fovy = fovy;
            standard_.z_near = z_near;
            standard_.z_far = z_far;
            standard_uniform_buffer_.set_data(standard_);
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
                auto mesh = STATIC_MESH_PTR(static_meshes_, mesh_instance.mesh);
                instance_matrices matrices;
                matrices.model_matrix = txform.matrix;
                matrices.model_view_matrix = standard_.view_matrix * matrices.model_matrix;
                matrices.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices.model_matrix))); //glm::transpose(glm::inverse(glm::mat3(matrices.model_view_matrix)));

                for (unsigned int i = 0; i < mesh->data.materials.size(); ++i) {
                    auto mat = MATERIAL_PTR(materials_, mesh->data.materials[i]);
                    auto tech = TECHNIQUE_PTR(techniques_, mat->data.technique_id);

                    // TODO allow overriding materials
                    // auto mit = mesh_instance.materials.find(i);
                    // if (mit != mesh_instance.materials.end()) {
                    //     mat = MATERIAL_PTR(materials_, mit->second);
                    // }

                    if (mat->data.transparent == transparent) {
                        tech->bind(textures_, cubemaps_, mat->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
                        tech->set_instance_matrices(&matrices);
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
            point_light_pass(viewport, world);

            // Render spot lights
            spot_light_pass(viewport, world);
        }

        template <class World>
        void image_based_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO IBL breaks energy conservation of the environment map
            // when transparancies are rendered.

            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());
            gbuffer_.bind_for_geometry_read();

            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));

            auto image_based_light_effect = EFFECT_PTR(effects_, settings_.image_based_light_effect);
            auto image_based_light_tech = TECHNIQUE_PTR(techniques_, image_based_light_effect->data.technique_id);
            image_based_light_tech->bind(textures_, cubemaps_, image_based_light_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
            STATIC_MESH_PTR(static_meshes_, image_based_light_effect->data.mesh)->render_all();
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
            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            frustum cascade_frustums[sbuffer_.count()];

            world.template for_each<transform, graphics::directional_light>([&](entity e, const transform& txform, const graphics::directional_light& light) {
                if (save_frustums) {
                    save_frustums = false;
                    debug_frustums.clear();
                    frustum scaled_frustum{
                        viewport.view_frustum.fovy(),
                        viewport.view_frustum.aspect(),
                        viewport.view_frustum.z_near(),
                        0.1f * viewport.view_frustum.z_far(),
                        viewport.view_frustum.view()
                    };
                    calculate_cascade_frustums(scaled_frustum, cascade_frustums, sbuffer_.count());

                    auto full_position = viewport.view_frustum.center();
                    auto full_target = full_position + light.direction;
                    auto light_view = glm::lookAt(full_target, full_position, glm::vec3{ 0, 1, 0 });

                    float minZ, maxZ;
                    viewport.view_frustum.full_light_projection(light_view, minZ, maxZ);

                    for (std::size_t i = 0; i < sbuffer_.count(); ++i) {
                        auto light_projection = cascade_frustums[i].clip_light_projection(light_view, minZ, maxZ);
                        debug_frustums.emplace_back(glm::vec3{ 1, 0, 0 }, glm::inverse(cascade_frustums[i].projection_view()));
                        debug_frustums.emplace_back(glm::vec3{ 1, 1, 0 }, glm::inverse(light_projection * light_view));
                    }
                }

                calculate_cascade_frustums(viewport.view_frustum, cascade_frustums, sbuffer_.count());

                auto full_position = viewport.view_frustum.center();
                auto full_target = full_position + light.direction;
                auto light_view = glm::lookAt(full_target, full_position, glm::vec3{ 0, 1, 0 });

                float minZ, maxZ;
                viewport.view_frustum.full_light_projection(light_view, minZ, maxZ);

                for (std::size_t i = 0; i < sbuffer_.count(); ++i) {
                    auto light_projection = cascade_frustums[i].clip_light_projection(light_view, minZ, maxZ);
                    shadow_.light_projection_view_matrix[i] = light_projection * light_view;
                    shadow_.light_frustum_far_plane[i] = cascade_frustums[i].far_plane();

                    setup_view_projection(size_, 1.5708f, 0.0, 0.0, light_view, light_projection);
                    render_to_shadow_map(i, world, light.cast_shadows);
                }

                setup_view_projection(size_,
                    viewport.view_frustum.fovy(),
                    viewport.view_frustum.z_near(),
                    viewport.view_frustum.z_far(),
                    viewport.view_frustum.view(),
                    viewport.view_frustum.projection());

                shadow_uniform_buffer_.set_data(shadow_);
                shadow_uniform_buffer_.set_binding_point(1);

                gbuffer_.bind_for_geometry_read();
                sbuffer_.bind_for_shadow_read(geometry_buffer::SHADOW_MAP0_TEXTURE_UINT);

                analytical_light_setup();

                GL_CHECK(glDisable(GL_DEPTH_TEST));
                GL_CHECK(glDisable(GL_CULL_FACE));

                auto directional_light_effect = EFFECT_PTR(effects_, settings_.directional_light_effect);
                auto directional_light_tech = TECHNIQUE_PTR(techniques_, directional_light_effect->data.technique_id);
                directional_light_tech->bind(textures_, cubemaps_, directional_light_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
                directional_light_tech->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                directional_light_tech->set_uniform("direction", light.direction);
                STATIC_MESH_PTR(static_meshes_, directional_light_effect->data.mesh)->render_all();
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

            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            gbuffer_.bind_for_geometry_read();

            analytical_light_setup();

            GL_CHECK(glDepthFunc(GL_GREATER));
            GL_CHECK(glEnable(GL_DEPTH_TEST));

            GL_CHECK(glCullFace(GL_FRONT));
            GL_CHECK(glEnable(GL_CULL_FACE));

            auto point_light_effect = EFFECT_PTR(effects_, settings_.point_light_effect);
            auto point_light_tech = TECHNIQUE_PTR(techniques_, point_light_effect->data.technique_id);
            point_light_tech->bind(textures_, cubemaps_, point_light_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);

            world.template for_each<transform, graphics::point_light>([&](entity e, const transform& txform, const graphics::point_light& light) {
                point_light_tech->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                point_light_tech->set_uniform("position_radius", glm::vec4(txform.position, txform.scale.x));
                STATIC_MESH_PTR(static_meshes_, point_light_effect->data.mesh)->render_all();
            });
        }

        template <class World>
        void spot_light_pass(const graphics::view_port& viewport, World& world)
        {
            // TODO:perf render cones for spot lights to limit there effects.
            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            world.template for_each<transform, graphics::spot_light>([&](entity e, transform& txform, const graphics::spot_light& light) {
                setup_view_projection(size_,
                    light.shadow_frustum.fovy(),
                    light.shadow_frustum.z_near(),
                    light.shadow_frustum.z_far(),
                    light.shadow_frustum.view(),
                    light.shadow_frustum.projection());

                render_to_shadow_map(0, world, light.cast_shadows);

                setup_view_projection(size_,
                    viewport.view_frustum.fovy(),
                    viewport.view_frustum.z_near(),
                    viewport.view_frustum.z_far(),
                    viewport.view_frustum.view(),
                    viewport.view_frustum.projection());

                shadow_.light_projection_view_matrix[0] = light.shadow_frustum.projection_view();
                shadow_uniform_buffer_.set_data(shadow_);
                shadow_uniform_buffer_.set_binding_point(1);

                gbuffer_.bind_for_geometry_read();
                sbuffer_.bind_for_shadow_read(geometry_buffer::SHADOW_MAP0_TEXTURE_UINT);

                analytical_light_setup();

                GL_CHECK(glDisable(GL_DEPTH_TEST));
                GL_CHECK(glDisable(GL_CULL_FACE));

                auto spot_light_effect = EFFECT_PTR(effects_, settings_.spot_light_effect);
                auto spot_light_tech = TECHNIQUE_PTR(techniques_, spot_light_effect->data.technique_id);
                spot_light_tech->bind(textures_, cubemaps_, spot_light_effect->data, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
                spot_light_tech->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                spot_light_tech->set_uniform("position", txform.position);
                spot_light_tech->set_uniform("direction", light.direction);
                spot_light_tech->set_uniform("cutoff", std::cos(light.cutoff));
                STATIC_MESH_PTR(static_meshes_, spot_light_effect->data.mesh)->render_all();
            });
        }

        template <class World>
        void render_to_shadow_map(int index, World& world, bool cast_shadows = true)
        {
            sbuffer_.bind_for_shadow_write(index);

            GL_CHECK(glDisable(GL_BLEND));

            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glDepthFunc(GL_LESS));
            GL_CHECK(glEnable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_STENCIL_TEST));

            GL_CHECK(glCullFace(GL_BACK));
            GL_CHECK(glEnable(GL_CULL_FACE));

            GL_CHECK(glClearColor(1.0f, 1.0f, 0.0f, 0.0f));
            GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

            auto shadow_technique = TECHNIQUE_PTR(techniques_, settings_.shadow_technique);
            shadow_technique->bind();

            if (cast_shadows) {
                world.template for_each<transform, graphics::static_mesh_instance>([&](entity e, const transform& txform, graphics::static_mesh_instance& mesh_instance) {
                    if (mesh_instance.cast_shadows) {
                        auto mesh = STATIC_MESH_PTR(static_meshes_, mesh_instance.mesh);
                        instance_matrices matrices;
                        matrices.model_matrix = txform.matrix;
                        matrices.model_view_matrix = standard_.view_matrix * matrices.model_matrix;

                        shadow_technique->set_instance_matrices(&matrices);

                        mesh->render_all();
                    }
                });
            }

            GL_CHECK(glDepthMask(GL_FALSE));
            GL_CHECK(glStencilMask(0x00));
        }

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
