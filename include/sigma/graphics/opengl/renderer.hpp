#ifndef SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/graphics/renderer.hpp>

#include <sigma/config.hpp>
#include <sigma/entity.hpp>
#include <sigma/graphics/opengl/cubemap.hpp>
#include <sigma/graphics/opengl/frame_buffer.hpp>
#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/material.hpp>
#include <sigma/graphics/opengl/post_process_effect.hpp>
#include <sigma/graphics/opengl/render_uniforms.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/uniform_buffer.hpp>

namespace sigma {

namespace opengl {
    struct mesh_instance_data {
        glm::mat4 model_matrix;
    };

    struct mesh_bucket {
        resource::handle<graphics::static_mesh> active_mesh;
        unsigned int material_slot;
        std::vector<mesh_instance_data> instances;
    };

    struct material_bucket {
        resource::handle<graphics::material> active_material;
        std::vector<mesh_bucket> mesh_buckets;
    };

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
            standard_uniform_data_.projection_matrix = viewport.projection_matrix;
            standard_uniform_data_.view_matrix = viewport.view_matrix;
            standard_uniform_data_.inverse_projection_matrix = glm::inverse(viewport.projection_matrix);
            standard_uniform_data_.inverse_view_matrix = glm::inverse(viewport.view_matrix);
            standard_uniform_data_.view_port_size = viewport.size;
            standard_uniform_data_.time = 0; // TODO time
            standard_uniform_data_.z_near = viewport.z_near;
            standard_uniform_data_.z_far = viewport.z_far;
            //standard_uniforms_.set_data(standard_uniform_data_);

            // Begin rendering

            instance_matrices matrices_;
            matrices_.model_matrix = glm::mat4(1);
            matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_view_matrix;
            matrices_.normal_matrix = glm::mat3(1);

            GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GL_CHECK(glClearStencil(0));

            // Opaque objects
            gbuffer_.bind_for_geometry_write();
            GL_CHECK(glDepthMask(GL_TRUE));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            geometry_pass(viewport, world, false);

            gbuffer_.bind_for_geometry_read();

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
            // EFFECT_PTR(texture_blit_effect_)->set_instance_matrices(&matrices_);
            // EFFECT_PTR(texture_blit_effect_)->apply();

            // Render final effects
            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));

            // gbuffer_.swap_input_image();
            // gbuffer_.bind_for_geometry_read();
            // EFFECT_PTR(vignette_effect_)->bind();
            // EFFECT_PTR(vignette_effect_)->set_standard_uniforms(&standard_uniform_data_);
            // EFFECT_PTR(vignette_effect_)->set_instance_matrices(&matrices_);
            // EFFECT_PTR(vignette_effect_)->apply();

            gbuffer_.swap_input_image();
            gbuffer_.bind_for_geometry_read();

            default_fbo_.bind(frame_buffer::target::DRAW);
            GL_CHECK(glClearColor(0, 0, 0, 1));
            GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

            EFFECT_CONST_PTR(gamma_conversion_)->bind();
            EFFECT_CONST_PTR(gamma_conversion_)->set_standard_uniforms(&standard_uniform_data_);
            EFFECT_CONST_PTR(gamma_conversion_)->set_instance_matrices(&matrices_);
            EFFECT_CONST_PTR(gamma_conversion_)->apply();
        }

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;

        int loader_status_;
        default_frame_buffer default_fbo_;
        geometry_buffer gbuffer_;
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
        resource::handle<graphics::post_process_effect> texture_blit_effect_;

        resource::handle<graphics::post_process_effect> gamma_conversion_;

        resource::handle<graphics::post_process_effect> vignette_effect_;

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

            world.template for_each<transform, graphics::static_mesh_instance>([&](entity e, transform& txform, graphics::static_mesh_instance& mesh_instance) {
                auto mesh = STATIC_MESH_PTR(mesh_instance.mesh);
                instance_matrices matrices;
                matrices.model_matrix = txform.matrix();
                matrices.model_view_matrix = standard_uniform_data_.view_matrix * matrices.model_matrix;
                matrices.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices.model_view_matrix)));

                for (unsigned int i = 0; i < mesh->material_count(); ++i) {
                    auto mat = MATERIAL_PTR(mesh->material(i));
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

        //void render_material_buckets(std::vector<material_bucket>& material_buckets);

        template <class World>
        void light_pass(const graphics::view_port& viewport, World& world)
        {
            // GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
            // Render Image based lighting
            // TODO IBL breaks energy conservation of the environment map
            // when transparancies are rendered.
            instance_matrices matrices_;
            matrices_.model_matrix = glm::mat4(1);
            matrices_.model_view_matrix = viewport.view_matrix * matrices_.model_view_matrix;
            matrices_.normal_matrix = glm::mat3(1);

            GL_CHECK(glDisable(GL_BLEND));
            GL_CHECK(glDisable(GL_STENCIL_TEST));
            GL_CHECK(glDisable(GL_DEPTH_TEST));

            EFFECT_CONST_PTR(image_based_light_effect_)->bind();
            EFFECT_CONST_PTR(image_based_light_effect_)->set_standard_uniforms(&standard_uniform_data_);
            EFFECT_CONST_PTR(image_based_light_effect_)->set_instance_matrices(&matrices_);
            EFFECT_CONST_PTR(image_based_light_effect_)->apply();

            // Setup state for analytical lights
            GL_CHECK(glStencilMask(0x00));
            GL_CHECK(glStencilFunc(GL_EQUAL, 1, 0xFF));
            GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
            GL_CHECK(glEnable(GL_STENCIL_TEST));

            GL_CHECK(glBlendEquation(GL_FUNC_ADD));
            GL_CHECK(glBlendFunc(GL_ONE, GL_ONE));
            GL_CHECK(glEnable(GL_BLEND));

            // TODO:perf look into passing all analytical lights(directional,point,spot) into one shader
            // instead of rendering the geometry for each. This not only would reduce the number of polygons
            // drawn but more inportantly would mean that the gbuffer would only have to be sampled once per
            // screen pixel.

            // Render point lights
            GL_CHECK(glCullFace(GL_FRONT));
            GL_CHECK(glEnable(GL_CULL_FACE));

            GL_CHECK(glDepthFunc(GL_GREATER));
            GL_CHECK(glEnable(GL_DEPTH_TEST));

            EFFECT_CONST_PTR(point_light_effect_)->bind();
            EFFECT_CONST_PTR(point_light_effect_)->set_standard_uniforms(&standard_uniform_data_);

            // TODO:perf re-incorporate the stencil test that limits the pixels being light see
            // http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
            // TODO:perf make sure that using instanceing is a win here, if it is not go back to
            // standard drawing methods.
            // TODO:perf look into using a quad for rendering point lights.
            // TODO:perf look into using a fullscreen quad for all point lights and have just one pass
            // that does all point light lighting at once.

            world.template for_each<transform, graphics::point_light>([&](entity e, transform& txform, graphics::point_light& light) {
                EFFECT_PTR(point_light_effect_)->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                EFFECT_PTR(point_light_effect_)->set_uniform("position_radius", glm::vec4(txform.position(), txform.scale().x));
                EFFECT_PTR(point_light_effect_)->bind(); // TODO update uniforms
                EFFECT_PTR(point_light_effect_)->apply();
            });

            // TODO Render spot lights

            // Render directional lights
            GL_CHECK(glDisable(GL_DEPTH_TEST));
            GL_CHECK(glDisable(GL_CULL_FACE));

            EFFECT_CONST_PTR(directional_light_effect_)->bind();
            EFFECT_CONST_PTR(directional_light_effect_)->set_standard_uniforms(&standard_uniform_data_);

            // // TODO:perf we can use one fullscreen quad to render all of the directional lights and save on gbuffer lookups.

            world.template for_each<transform, graphics::directional_light>([&](entity e, transform& txform, graphics::directional_light& light) {
                EFFECT_PTR(directional_light_effect_)->set_uniform("color_intensity", glm::vec4(light.color, light.intensity));
                EFFECT_PTR(directional_light_effect_)->set_uniform("direction", glm::vec3(txform.matrix() * glm::vec4(0, 1, 0, 0)));
                EFFECT_PTR(directional_light_effect_)->bind(); // TODO update uniforms
                EFFECT_PTR(directional_light_effect_)->apply();
            });
        }

        // void point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius);

        // std::size_t fill_material_buckets(const graphics::view_port& viewport, std::vector<material_bucket>& material_buckets, bool transparent);
        //
        // void setup_instanced_material_buckets(std::vector<material_bucket>& material_buckets);
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CONTEXT_HPP
