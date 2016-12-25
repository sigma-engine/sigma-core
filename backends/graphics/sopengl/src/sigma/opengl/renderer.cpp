#include <sigma/opengl/renderer.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include <iostream>

namespace sigma {
namespace opengl {
    renderer::renderer(context* ctx, glm::ivec2 size)
        : graphics::renderer(ctx, size)
        , ctx_(ctx)
        , default_fbo_(size)
        , position_texture_(internal_format::RGB32F, size)
        , diffuse_texture_(internal_format::RGB32F, size)
        , normal_texture_(internal_format::RGB32F, size)
        , texcoord_texture_(internal_format::RGB32F, size)
        , depth_texture_(internal_format::DEPTH_COMPONENT32F, size)
        , gbuffer_(size)
        , textures_(ctx_->textures())
        , shaders_(ctx_->shaders())
        , materials_(ctx_->materials(), textures_, shaders_)
        , static_meshes_(ctx_->static_meshes(), materials_)
    {
        std::cout << glGetString(GL_VERSION) << std::endl;
        gbuffer_.attach(frame_buffer_attachment::COLOR_ATTACHMENT0, position_texture_);
        gbuffer_.attach(frame_buffer_attachment::COLOR_ATTACHMENT1, diffuse_texture_);
        gbuffer_.attach(frame_buffer_attachment::COLOR_ATTACHMENT2, normal_texture_);
        gbuffer_.attach(frame_buffer_attachment::COLOR_ATTACHMENT3, texcoord_texture_);
        gbuffer_.attach(frame_buffer_attachment::DEPTH_ATTACHMENT, depth_texture_);
        gbuffer_.draw_buffers({ frame_buffer_attachment::COLOR_ATTACHMENT0,
            frame_buffer_attachment::COLOR_ATTACHMENT1,
            frame_buffer_attachment::COLOR_ATTACHMENT2,
            frame_buffer_attachment::COLOR_ATTACHMENT3 });
    }

    renderer::~renderer()
    {
    }

    void renderer::resize(glm::uvec2 size)
    {
        GL_CHECK(glViewport(0, 0, size.x, size.y));
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        gbuffer_.bind_for_writting();
        matrices_.projection_matrix = viewport.projection_matrix;
        matrices_.view_matrix = viewport.view_matrix;

        GL_CHECK(glClearColor(0.8f, 0.8f, 0.8f, 1.0f));
        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glDisable(GL_BLEND));
        GL_CHECK(glCullFace(GL_BACK));
        GL_CHECK(glDisable(GL_CULL_FACE));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.static_mesh_instances.has(e) && viewport.transforms.has(e)) {
                const auto& txform = viewport.transforms.get(e);
                auto mesh = static_meshes_.get(viewport.static_mesh_instances.get(e));

                glm::mat4 model_matrix(1.0f);
                model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(model_matrix, txform.scale), txform.position);

                matrices_.model_view_matrix = viewport.view_matrix * model_matrix;
                matrices_.normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrices_.model_view_matrix)));
                mesh->render(&matrices_);
            }
        }

        default_fbo_.bind();
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        gbuffer_.bind_for_reading();

        GL_CHECK(glReadBuffer(GLenum(frame_buffer_attachment::COLOR_ATTACHMENT1)));
        glm::ivec2 s = gbuffer_.size();
        glBlitFramebuffer(0, 0, s.x, s.y, 0, 0, s.x, s.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
}
}

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)
