#include <sigma/graphics/opengl/renderer.hpp>

#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

#include <iostream>
#include <stdexcept>

#define MODEL_MATRIX_NAME "model_matrix"
#define MODEL_VIEW_MATRIX_NAME "model_view_matrix"
#define NORMAL_MATRIX_NAME "normal_matrix"

namespace sigma {
namespace opengl {
    void calculate_cascade_frustums(const frustum& view_frustum, std::vector<frustum>& cascade_frustums)
    {
        // TODO move this into viewport or
        // what ever viewport gets refactored into.

        auto count = cascade_frustums.size();
        std::vector<float> cascade_splits(count + 1);

        const float lambda = 0.7f;
        const float n = view_frustum.z_near();
        const float f = view_frustum.z_far();
        for (std::size_t i = 0; i < count; ++i) {
            float fi = float(i) / count;
            float Ci_log = n * std::pow(f / n, fi);
            float Ci_uni = n + (f - n) * fi;
            cascade_splits[i] = lambda * Ci_log + (1 - lambda) * Ci_uni;
        }
        cascade_splits[count] = f;

        for (std::size_t i = 0; i < count; ++i) {
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
        , default_fbo_(0)
        , cascade_frustums_(3)
        , start_time_(std::chrono::high_resolution_clock::now())
        , textures_(ctx.get_cache<graphics::texture>())
        , cubemaps_(ctx.get_cache<graphics::texture>(), ctx.get_cache<graphics::cubemap>())
        , shaders_(ctx.get_cache<graphics::shader>())
        , techniques_(ctx.get_cache<graphics::technique>(), shaders_)
        , static_meshes_(ctx.get_cache<graphics::static_mesh>())
        , materials_(ctx.get_cache<graphics::material>())
        , effects_(ctx.get_cache<graphics::post_process_effect>())
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        // TODO disable this in production.
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debug_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);

        create_geometry_buffer(size_);
        create_shadow_maps({ 1024, 1024 });

        debug_renderer_.windowWidth = size.x;
        debug_renderer_.windowHeight = size.y;
        dd::initialize(&debug_renderer_);

        standard_uniform_buffer_.set_binding_point(0);
    }

    renderer::~renderer()
    {
        dd::shutdown();
        destroy_shadow_maps();
        destroy_geometry_buffer();
    }

    void renderer::resize(glm::uvec2 size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    void renderer::render(const graphics::view_port& viewport, const graphics::renderer::world_view_type& world)
    {
        setup_view_projection(size_,
            viewport.view_frustum.fovy(),
            viewport.view_frustum.z_near(),
            viewport.view_frustum.z_far(),
            viewport.view_frustum.view(),
            viewport.view_frustum.projection());

        // Opaque objects
        geometry_pass(viewport, world, false);
        light_pass(viewport, world);

        // TODO Transparent objects

        glDisable(GL_BLEND);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        if (settings_.enable_debug_rendering) {
            debug_renderer_.mvpMatrix = viewport.view_frustum.projection_view();
            for (const auto& f : debug_frustums_)
                dd::frustum(glm::value_ptr(f.second), glm::value_ptr(f.first));

            glm::vec3 debug_color{ 0, 1, 1 };
            world.for_each<transform, graphics::point_light>([&](const auto& e, const auto& txform, const auto& point) {
                dd::sphere(glm::value_ptr(txform.position), glm::value_ptr(debug_color), txform.scale.x);
            });

            world.for_each<transform, graphics::spot_light>([&](const auto& e, const auto& txform, const auto& spot) {
                float scale = 10;
                glm::vec3 dir = -scale * spot.direction;
                dd::cone(glm::value_ptr(txform.position),
                    glm::value_ptr(dir),
                    glm::value_ptr(debug_color), scale * std::tan(spot.cutoff), 0);
            });

            // world.for_each<transform, graphics::static_mesh_instance>([&](entity e, const transform& txform, const graphics::static_mesh_instance& mesh_instance) {
            //     auto mesh = context_.get_cache<graphics::static_mesh>().acquire(mesh_instance.mesh);
            //     dd::sphere(glm::value_ptr(txform.position), glm::value_ptr(debug_color), mesh->radius);
            // });

            dd::flush(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time_).count());
        }

        // Render final effects
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        // gbuffer_.swap_input_image();
        // gbuffer_.bind_for_geometry_read();
        // auto vignette_effect = effects_.acquire(settings_.vignette_effect);
        // auto vignette_effect_tech = TECHNIQUE_PTR(techniques_, vignette_effect->technique_id);
        // vignette_effect_tech->bind(textures_, cubemaps_, vignette_effect, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        // STATIC_MESH_PTR(static_meshes_, vignette_effect->mesh)->render_all();

        geometry_swap_input_image();
        bind_for_geometry_read();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, default_fbo_);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        auto gamma_conversion_effect = effects_.acquire(settings_.gamma_conversion);
        auto [cpu_gamma_conversion_effect, gpu_gamma_conversion_program] = bind_technique(
            gamma_conversion_effect->technique_id,
            gamma_conversion_effect,
            geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        draw_effect_mesh(gamma_conversion_effect);
    }

    void renderer::create_geometry_buffer(const glm::ivec2& size)
    {
        size_ = size;

        // TODO filp roughness and metalnes.
        // Generate diffuse and roughness texture.
        glGenTextures(1, &gbuffer_diffuse_texture_);
        glBindTexture(GL_TEXTURE_2D, gbuffer_diffuse_texture_);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, size_.x, size_.y);

        // Generate normal and metalness texture.
        glGenTextures(1, &gbuffer_normal_texture_);
        glBindTexture(GL_TEXTURE_2D, gbuffer_normal_texture_);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, size_.x, size_.y);

        // Generate depth and stencil texture.
        glGenTextures(1, &gbuffer_depth_stencil_texture_);
        glBindTexture(GL_TEXTURE_2D, gbuffer_depth_stencil_texture_);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH32F_STENCIL8, size_.x, size_.y);

        // Generate accumulation textures.
        gbuffer_accumulation_textures_.resize(2, 0);
        glGenTextures(gbuffer_accumulation_textures_.size(), gbuffer_accumulation_textures_.data());
        for (auto accumulation_texture : gbuffer_accumulation_textures_) {
            glBindTexture(GL_TEXTURE_2D, accumulation_texture);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, size_.x, size_.y);
        }

        // Generate the framebuffer.
        glGenFramebuffers(1, &gbuffer_fbo_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer_fbo_);

        // Attach the images to the framebuffer.
        glFramebufferTexture2D(GL_FRAMEBUFFER, geometry_buffer::DIFFUSE_ROUGHNESS_ATTACHMENT, GL_TEXTURE_2D, gbuffer_diffuse_texture_, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, geometry_buffer::NORMAL_METALNESS_ATTACHMENT, GL_TEXTURE_2D, gbuffer_normal_texture_, 0);
        for (std::size_t i = 0; i < gbuffer_accumulation_textures_.size(); ++i) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, geometry_buffer::IMAGE_ATTACHMENTS[i], GL_TEXTURE_2D, gbuffer_accumulation_textures_[i], 0);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbuffer_depth_stencil_texture_, 0);

        gbuffer_input_image_ = 0;
        gbuffer_output_image_ = 1;
    }

    void renderer::geometry_swap_input_image()
    {
        gbuffer_output_image_++;
        gbuffer_output_image_ %= 2;
        gbuffer_input_image_++;
        gbuffer_input_image_ %= 2;
    }

    void renderer::bind_for_geometry_read()
    {
        glActiveTexture(geometry_buffer::DIFFUSE_ROUGHNESS_TEXTURE_UINT);
        glBindTexture(GL_TEXTURE_2D, gbuffer_diffuse_texture_);

        glActiveTexture(geometry_buffer::NORMAL_METALNESS_TEXTURE_UINT);
        glBindTexture(GL_TEXTURE_2D, gbuffer_normal_texture_);

        glActiveTexture(geometry_buffer::DEPTH_STENCIL_TEXTURE_UINT);
        glBindTexture(GL_TEXTURE_2D, gbuffer_depth_stencil_texture_);

        glActiveTexture(geometry_buffer::INPUT_IMAGE_TEXTURE_UINT);
        glBindTexture(GL_TEXTURE_2D, gbuffer_accumulation_textures_[gbuffer_input_image_]);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer_fbo_);
        GLenum attachments[] = { geometry_buffer::IMAGE_ATTACHMENTS[gbuffer_output_image_] };
        glDrawBuffers(1, attachments);
        glViewport(0, 0, size_.x, size_.y);
    }

    void renderer::bind_for_geometry_write()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer_fbo_);
        GLenum attachments[] = { geometry_buffer::DIFFUSE_ROUGHNESS_ATTACHMENT,
            geometry_buffer::NORMAL_METALNESS_ATTACHMENT,
            geometry_buffer::IMAGE_ATTACHMENTS[gbuffer_output_image_] };
        glDrawBuffers(3, attachments);
        glViewport(0, 0, size_.x, size_.y);

        glActiveTexture(geometry_buffer::INPUT_IMAGE_TEXTURE_UINT);
        glBindTexture(GL_TEXTURE_2D, gbuffer_accumulation_textures_[gbuffer_input_image_]);
    }

    void renderer::destroy_geometry_buffer()
    {
        glDeleteFramebuffers(1, &gbuffer_fbo_);
        glDeleteTextures(gbuffer_accumulation_textures_.size(), gbuffer_accumulation_textures_.data());
        glDeleteTextures(1, &gbuffer_depth_stencil_texture_);
        glDeleteTextures(1, &gbuffer_normal_texture_);
        glDeleteTextures(1, &gbuffer_diffuse_texture_);
    }

    void renderer::create_shadow_maps(const glm::ivec2& size)
    {
        shadow_map_size_ = size;

        // TODO make this a paramater
        auto shadow_map_levels = 1 + std::floor(std::log2(std::max(size.x, size.y)));

        // Generate the textures for the shadow maps.
        shadow_textures_.resize(3, 0);
        glGenTextures(shadow_textures_.size(), shadow_textures_.data());

        // Allocate storage for the shadow maps.
        for (auto texture : shadow_textures_) {
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexStorage2D(GL_TEXTURE_2D, shadow_map_levels, GL_RG32F, size.x, size.y);
        }

        // Generate and allocate the depth buffer for the framebuffer.
        glGenRenderbuffers(1, &shadow_depth_buffer_);
        glBindRenderbuffer(GL_RENDERBUFFER, shadow_depth_buffer_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.x, size.y);

        // Generate the framebuffer and attach the depth buffer.
        glGenFramebuffers(1, &shadow_fbo_);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_fbo_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, shadow_depth_buffer_);
    }

    void renderer::bind_for_shadow_read()
    {
        for (std::size_t i = 0; i < shadow_textures_.size(); ++i) {
            glActiveTexture(geometry_buffer::SHADOW_MAP0_TEXTURE_UINT + i);
            glBindTexture(GL_TEXTURE_2D, shadow_textures_[i]);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void renderer::bind_for_shadow_write(unsigned int index)
    {
        static GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadow_fbo_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadow_textures_[index], 0);
        glDrawBuffers(1, attachments);
        glViewport(0, 0, shadow_map_size_.x, shadow_map_size_.y);
    }

    void renderer::destroy_shadow_maps()
    {
        glDeleteFramebuffers(1, &shadow_fbo_);
        glDeleteRenderbuffers(1, &shadow_depth_buffer_);
        glDeleteTextures(shadow_textures_.size(), shadow_textures_.data());
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

    void renderer::geometry_pass(const graphics::view_port& viewport, const renderer::world_view_type& world, bool transparent)
    {
        bind_for_geometry_write();
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_BLEND);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        fill_render_token_stream(viewport.view_frustum, world, geometry_pass_token_stream_);
        sort_render_token_stream(geometry_pass_token_stream_);

        std::size_t last_batch = -1;
        for (auto& token : geometry_pass_token_stream_) {
            auto [cpu_technique, gpu_program] = bind_technique(
                token.material->technique_id,
                token.material,
                geometry_buffer::NEXT_FREE_TEXTURE_UINT);

            GLint model_loc = glGetUniformLocation(token.program, MODEL_MATRIX_NAME);
            GLint model_view_loc = glGetUniformLocation(token.program, MODEL_VIEW_MATRIX_NAME);
            GLint normal_loc = glGetUniformLocation(token.program, NORMAL_MATRIX_NAME);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.model_matrix));
            glUniformMatrix4fv(model_view_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.model_view_matrix));
            glUniformMatrix3fv(normal_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.normal_matrix));

            if (token.buffer.batch_index != last_batch)
                static_meshes_.bind_batch(token.buffer.batch_index);

            glDrawElementsBaseVertex(GL_TRIANGLES,
                token.count,
                GL_UNSIGNED_INT,
                (const void*)(sizeof(unsigned int) * (token.buffer.base_index + token.offset)),
                token.buffer.base_vertex);
        }

        geometry_pass_token_stream_.clear();

        glDepthMask(GL_FALSE);
    }

    void renderer::light_pass(const graphics::view_port& viewport, const renderer::world_view_type& world)
    {
        // glClear(GL_COLOR_BUFFER_BIT);

        // TODO:perf look into passing all analytical lights(directional,point,spot) into one shader
        // instead of rendering the geometry for each. This not only would reduce the number of polygons
        // drawn but more inportantly would mean that the gbuffer would only have to be sampled once per
        // screen pixel.

        if (settings_.enable_image_based_lighting) {
            // Render Image based lighting
            image_based_light_pass(viewport);
        }

        // Render directional lights
        directional_light_pass(viewport, world);

        // Render point lights
        point_light_pass(viewport, world);

        // Render spot lights
        spot_light_pass(viewport, world);
    }

    void renderer::image_based_light_pass(const graphics::view_port& viewport)
    {
        setup_view_projection(size_,
            viewport.view_frustum.fovy(),
            viewport.view_frustum.z_near(),
            viewport.view_frustum.z_far(),
            viewport.view_frustum.view(),
            viewport.view_frustum.projection());
        bind_for_geometry_read();

        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        auto image_based_light_effect = effects_.acquire(settings_.image_based_light_effect);
        auto [cpu_image_based_light_effect, gpu_image_based_program] = bind_technique(
            image_based_light_effect->technique_id,
            image_based_light_effect,
            geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        draw_effect_mesh(image_based_light_effect);
    }

    void renderer::analytical_light_setup()
    {
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        glEnable(GL_BLEND);
    }

    void renderer::directional_light_pass(const graphics::view_port& viewport, const renderer::world_view_type& world)
    {
        // TODO:perf we can use one fullscreen quad to render all of the directional lights and save on gbuffer lookups.
        setup_view_projection(size_,
            viewport.view_frustum.fovy(),
            viewport.view_frustum.z_near(),
            viewport.view_frustum.z_far(),
            viewport.view_frustum.view(),
            viewport.view_frustum.projection());

        world.for_each<transform, graphics::directional_light>([&](entity e, const transform& txform, const graphics::directional_light& light) {
            if (save_frustums) {
                save_frustums = false;
                debug_frustums_.clear();
                frustum scaled_frustum{
                    viewport.view_frustum.fovy(),
                    viewport.view_frustum.aspect(),
                    viewport.view_frustum.z_near(),
                    0.1f * viewport.view_frustum.z_far(),
                    viewport.view_frustum.view()
                };
                calculate_cascade_frustums(scaled_frustum, cascade_frustums_);

                auto full_position = viewport.view_frustum.center();
                auto full_target = full_position + light.direction;
                auto light_view = glm::lookAt(full_target, full_position, glm::vec3{ 0, 1, 0 });

                float minZ, maxZ;
                viewport.view_frustum.full_light_projection(light_view, minZ, maxZ);

                for (std::size_t i = 0; i < shadow_textures_.size(); ++i) {
                    auto light_projection = cascade_frustums_[i].clip_light_projection(light_view, minZ, maxZ);
                    debug_frustums_.emplace_back(glm::vec3{ 1, 0, 0 }, glm::inverse(cascade_frustums_[i].projection_view()));
                    debug_frustums_.emplace_back(glm::vec3{ 1, 1, 0 }, glm::inverse(light_projection * light_view));
                }
            }

            calculate_cascade_frustums(viewport.view_frustum, cascade_frustums_);

            auto full_position = viewport.view_frustum.center();
            auto full_target = full_position + light.direction;
            auto light_view = glm::lookAt(full_target, full_position, glm::vec3{ 0, 1, 0 });

            float minZ, maxZ;
            viewport.view_frustum.full_light_projection(light_view, minZ, maxZ);

            for (std::size_t i = 0; i < shadow_textures_.size(); ++i) {
                auto light_projection = cascade_frustums_[i].clip_light_projection(light_view, minZ, maxZ);
                shadow_.light_projection_view_matrix[i] = light_projection * light_view;
                shadow_.light_frustum_far_plane[i] = cascade_frustums_[i].far_plane();

                setup_view_projection(size_, 1.5708f, 0.0, 0.0, light_view, light_projection);
                render_to_shadow_map(cascade_frustums_[i], i, world, light.cast_shadows);
            }

            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            shadow_uniform_buffer_.set_data(shadow_);
            shadow_uniform_buffer_.set_binding_point(1);

            bind_for_geometry_read();
            bind_for_shadow_read();

            analytical_light_setup();

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            auto directional_light_effect = effects_.acquire(settings_.directional_light_effect);
            auto [cpu_directional_light_effect, gpu_directional_program] = bind_technique(
                directional_light_effect->technique_id,
                directional_light_effect,
                geometry_buffer::NEXT_FREE_TEXTURE_UINT);

            // TODO store these or use unifrom buffer object
            glm::vec4 color_intensity = glm::vec4(light.color, light.intensity);
            GLint color_loc = glGetUniformLocation(gpu_directional_program, "color_intensity");
            if (color_loc != -1)
                glUniform4fv(color_loc, 1, glm::value_ptr(color_intensity));

            GLint dir_loc = glGetUniformLocation(gpu_directional_program, "direction");
            if (dir_loc != -1)
                glUniform3fv(dir_loc, 1, glm::value_ptr(light.direction));

            draw_effect_mesh(directional_light_effect);
        });
    }

    void renderer::point_light_pass(const graphics::view_port& viewport, const renderer::world_view_type& world)
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

        bind_for_geometry_read();

        analytical_light_setup();

        glDepthFunc(GL_GREATER);
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);

        auto point_light_effect = effects_.acquire(settings_.point_light_effect);
        auto [cpu_point_tech, gpu_point_program] = bind_technique(
            point_light_effect->technique_id,
            point_light_effect,
            geometry_buffer::NEXT_FREE_TEXTURE_UINT);

        // TODO remove these or use uniform buffers
        GLint color_loc = glGetUniformLocation(gpu_point_program, "color_intensity");
        GLint pos_loc = glGetUniformLocation(gpu_point_program, "position_radius");

        world.for_each<transform, graphics::point_light>([&](entity e, const transform& txform, const graphics::point_light& light) {
            glm::vec4 color_intensity(light.color, light.intensity);
            glm::vec4 position_radius(txform.position, txform.scale.x);

            if (color_loc != -1)
                glUniform4fv(color_loc, 1, glm::value_ptr(color_intensity));
            if (pos_loc != -1)
                glUniform4fv(pos_loc, 1, glm::value_ptr(position_radius));

            draw_effect_mesh(point_light_effect);
        });
    }

    void renderer::spot_light_pass(const graphics::view_port& viewport, const renderer::world_view_type& world)
    {
        // TODO:perf render cones for spot lights to limit there effects.

        world.for_each<transform, graphics::spot_light>([&](entity e, const transform& txform, const graphics::spot_light& light) {
            setup_view_projection(size_,
                light.shadow_frustum.fovy(),
                light.shadow_frustum.z_near(),
                light.shadow_frustum.z_far(),
                light.shadow_frustum.view(),
                light.shadow_frustum.projection());

            render_to_shadow_map(light.shadow_frustum, 0, world, light.cast_shadows);

            setup_view_projection(size_,
                viewport.view_frustum.fovy(),
                viewport.view_frustum.z_near(),
                viewport.view_frustum.z_far(),
                viewport.view_frustum.view(),
                viewport.view_frustum.projection());

            shadow_.light_projection_view_matrix[0] = light.shadow_frustum.projection_view();
            shadow_uniform_buffer_.set_data(shadow_);
            shadow_uniform_buffer_.set_binding_point(1);

            bind_for_geometry_read();
            bind_for_shadow_read();

            analytical_light_setup();

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);

            auto spot_light_effect = effects_.acquire(settings_.spot_light_effect);
            auto [cpu_spot_effect, gpu_spot_program] = bind_technique(
                spot_light_effect->technique_id,
                spot_light_effect,
                geometry_buffer::NEXT_FREE_TEXTURE_UINT);

            // TODO store these or use unifrom buffer object
            glm::vec4 color_intensity = glm::vec4(light.color, light.intensity);
            GLint color_loc = glGetUniformLocation(gpu_spot_program, "color_intensity");
            if (color_loc != -1)
                glUniform4fv(color_loc, 1, glm::value_ptr(color_intensity));

            GLint pos_loc = glGetUniformLocation(gpu_spot_program, "position");
            if (pos_loc != -1)
                glUniform3fv(pos_loc, 1, glm::value_ptr(txform.position));

            GLint dir_loc = glGetUniformLocation(gpu_spot_program, "direction");
            if (dir_loc != -1)
                glUniform3fv(dir_loc, 1, glm::value_ptr(light.direction));

            GLint cutoff_loc = glGetUniformLocation(gpu_spot_program, "cutoff");
            if (cutoff_loc != -1)
                glUniform1f(cutoff_loc, std::cos(light.cutoff));

            draw_effect_mesh(spot_light_effect);
        });
    }

    void renderer::render_to_shadow_map(const frustum& view_frustum, int index, const renderer::world_view_type& world, bool cast_shadows)
    {
        bind_for_shadow_write(index);

        glDisable(GL_BLEND);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        if (cast_shadows) {
            auto [cpu_technique, gpu_program] = techniques_.acquire(settings_.shadow_technique);
            glUseProgram(gpu_program);

            // TODO store theses or use uniform buffers
            GLint model_loc = glGetUniformLocation(gpu_program, MODEL_MATRIX_NAME);
            GLint model_view_loc = glGetUniformLocation(gpu_program, MODEL_VIEW_MATRIX_NAME);
            GLint normal_loc = glGetUniformLocation(gpu_program, NORMAL_MATRIX_NAME);

            fill_render_token_stream(view_frustum, world, shadow_map_token_stream_, gpu_program);
            sort_render_token_stream(shadow_map_token_stream_);

            std::size_t last_batch = -1;
            for (auto& token : shadow_map_token_stream_) {
                glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.model_matrix));
                glUniformMatrix4fv(model_view_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.model_view_matrix));
                glUniformMatrix3fv(normal_loc, 1, GL_FALSE, glm::value_ptr(token.matrices.normal_matrix));

                if (token.buffer.batch_index != last_batch)
                    static_meshes_.bind_batch(token.buffer.batch_index);

                glDrawElementsBaseVertex(GL_TRIANGLES,
                    token.count,
                    GL_UNSIGNED_INT,
                    (const void*)(sizeof(unsigned int) * (token.buffer.base_index + token.offset)),
                    token.buffer.base_vertex);
            }

            shadow_map_token_stream_.clear();
        }

        glDepthMask(GL_FALSE);
    }

    void renderer::fill_render_token_stream(const frustum& view, const world_view_type& world, std::vector<render_token>& tokens, GLuint global_program)
    {
        auto view_matrix = view.view();
        world.for_each<transform, graphics::static_mesh_instance>([&](const entity& e, const transform& txform, const graphics::static_mesh_instance& mesh_instance) {
            auto [cpu_mesh, gpu_mesh] = static_meshes_.acquire(mesh_instance.mesh);
            if (view.contains_sphere(txform.position, cpu_mesh->radius)) {
                auto model_view_matrix = view_matrix * txform.matrix;
                auto normal_matrix = glm::transpose(glm::inverse(glm::mat3(txform.matrix)));
                if (global_program) {
                    render_token tok;
                    tok.program = global_program;
                    tok.buffer = gpu_mesh;
                    tok.offset = 0;
                    tok.count = 3 * cpu_mesh->triangles.size();
                    tok.matrices.model_matrix = txform.matrix;
                    tok.matrices.model_view_matrix = model_view_matrix;
                    tok.matrices.normal_matrix = normal_matrix;

                    tokens.push_back(std::move(tok));
                } else {
                    for (unsigned int i = 0; i < cpu_mesh->material_slots.size(); ++i) {
                        auto material = materials_.acquire(cpu_mesh->materials[i]);
                        auto [cpu_technique, gpu_program] = techniques_.acquire(material->technique_id);

                        render_token tok;
                        tok.program = gpu_program;
                        tok.buffer = gpu_mesh;
                        tok.offset = 3 * cpu_mesh->material_slots[i].first;
                        tok.count = 3 * cpu_mesh->material_slots[i].second;
                        tok.matrices.model_matrix = txform.matrix;
                        tok.matrices.model_view_matrix = model_view_matrix;
                        tok.matrices.normal_matrix = normal_matrix;
                        tok.material = material;

                        tokens.push_back(std::move(tok));
                    }
                }
            }
        });
    }

    void renderer::sort_render_token_stream(std::vector<render_token>& tokens)
    {
        std::sort(tokens.begin(), tokens.end(), [](const render_token& a, const render_token& b) {
            if (a.program == b.program) {
                // TODO this sort order seams weird but it seams to run faster
                if (a.buffer.batch_index == b.buffer.batch_index)
                    return a.material < b.material;

                return a.buffer.batch_index < b.buffer.batch_index;
            }

            return a.program < b.program;
        });
    }

    std::pair<graphics::technique*, GLuint> renderer::bind_technique(
        const resource::handle<graphics::technique>& technique,
        const graphics::technique_uniform_data* data,
        GLenum first_texture_unit)
    {
        auto [cpu_technique, gpu_program] = techniques_.acquire(technique);
        glUseProgram(gpu_program);

        for (const auto& [name, value] : data->floats) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1)
                glUniform1f(loc, value);
        }

        for (const auto& [name, value] : data->vec2s) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1)
                glUniform2fv(loc, 1, glm::value_ptr(value));
        }

        for (const auto& [name, value] : data->vec3s) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1)
                glUniform3fv(loc, 1, glm::value_ptr(value));
        }

        for (const auto& [name, value] : data->vec4s) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1)
                glUniform4fv(loc, 1, glm::value_ptr(value));
        }

        auto texture_unit = first_texture_unit;
        auto unit_number = GLenum(first_texture_unit) - GL_TEXTURE0;

        for (const auto& [name, value] : data->textures) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1) {
                glActiveTexture(texture_unit);
                glBindTexture(GL_TEXTURE_2D, textures_.acquire(value).second);
                glUniform1i(loc, unit_number);
                unit_number++;
                texture_unit++;
            }
        }

        for (const auto& [name, value] : data->cubemaps) {
            // TODO store these or use uniform buffers
            GLint loc = glGetUniformLocation(gpu_program, name.c_str());
            if (loc != -1) {
                glActiveTexture(texture_unit);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps_.acquire(value).second);
                glUniform1i(loc, unit_number);
                unit_number++;
                texture_unit++;
            }
        }

        return { cpu_technique, gpu_program };
    }

    void renderer::draw_effect_mesh(graphics::post_process_effect* effect)
    {
        auto [cpu_mesh, gpu_mesh] = static_meshes_.acquire(effect->mesh);

        static_meshes_.bind_batch(gpu_mesh.batch_index);
        glDrawElementsBaseVertex(GL_TRIANGLES,
            3 * cpu_mesh->triangles.size(),
            GL_UNSIGNED_INT,
            (const void*)(sizeof(unsigned int) * gpu_mesh.base_index),
            gpu_mesh.base_vertex);
    }

    /*void renderer::point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius)
    {
        // http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
        if (glm::length(view_space_position) > 1.1 * radius) {
            gbuffer_.bind_for_stencil_pass();

            glEnable(GL_STENCIL_TEST);
            glClearStencil(4);
            glClear(GL_STENCIL_BUFFER_BIT);

            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_GEQUAL, 6, 0xFF);

            glDepthFunc(GL_GEQUAL);
            glCullFace(GL_FRONT);

            EFFECT_PTR(point_light_stencil_effect_)->apply(&matrices_);

            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glStencilFunc(GL_LEQUAL, 5, 0xFF);

            glDepthFunc(GL_LEQUAL);
            glCullFace(GL_BACK);
        }
    }*/

    void debug_callback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "severity: LOW\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "severity: MEDIUM\n";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "severity: HIGH\n";
            break;
        default:
            return;
        }

        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "type: ERROR\n";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "type: DEPRECATED_BEHAVIOR\n";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "type: UNDEFINED_BEHAVIOR\n";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "type: PORTABILITY\n";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "type: PERFORMANCE\n";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "type: MARKER\n";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "type: PUSH_GROUP\n";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "type: POP_GROUP\n";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "type: OTHER\n";
            break;
        }
        std::cout << "message: " << message << '\n';
        std::cout << "id: " << id << '\n';
        switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "source: API\n";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "source: WINDOW_SYSTEM\n";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "source: SHADER_COMPILER\n";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "source: THIRD_PARTY\n";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "source: APPLICATION\n";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "source: OTHER\n";
            break;
        }
    }
}
}
