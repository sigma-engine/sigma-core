#include <sigma/graphics/opengl/geometry_buffer.hpp>

#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    constexpr const frame_buffer::attachment geometry_buffer::IMAGE_ATTACHMENTS[];

    geometry_buffer::geometry_buffer(glm::ivec2 size)
        : frame_buffer(size)
        , diffuse_texture_(internal_format::RGBA8, size) // TODO filp roughness and metalness
        , normal_texture_(internal_format::RGBA16F, size) // RGB10_A2
        , depth_stencil_texture_(internal_format::DEPTH32F_STENCIL8, size)
        , images_{ texture{ internal_format::RGB16F, size }, texture{ internal_format::RGB16F, size } }
        , input_image_(0)
        , output_image_(1)
    {
        attach(DIFFUSE_ROUGHNESS_ATTACHMENT, diffuse_texture_);
        attach(NORMAL_METALNESS_ATTACHMENT, normal_texture_);
        attach(IMAGE_ATTACHMENTS[0], images_[0]);
        attach(IMAGE_ATTACHMENTS[1], images_[1]);
        attach(frame_buffer::attachment::DEPTH_STENCIL, depth_stencil_texture_);
    }

    void geometry_buffer::bind_for_geometry_write()
    {
        draw_buffers(DIFFUSE_ROUGHNESS_ATTACHMENT, NORMAL_METALNESS_ATTACHMENT, IMAGE_ATTACHMENTS[output_image_]);
        GL_CHECK(glActiveTexture(GLenum(INPUT_IMAGE_TEXTURE_UINT)));
        images_[input_image_].bind();
    }

    void geometry_buffer::bind_for_geometry_read()
    {
        draw_buffers(IMAGE_ATTACHMENTS[output_image_]);

        GL_CHECK(glActiveTexture(GLenum(DIFFUSE_ROUGHNESS_TEXTURE_UINT)));
        diffuse_texture_.bind();

        GL_CHECK(glActiveTexture(GLenum(NORMAL_METALNESS_TEXTURE_UINT)));
        normal_texture_.bind();

        GL_CHECK(glActiveTexture(GLenum(DEPTH_STENCIL_TEXTURE_UINT)));
        depth_stencil_texture_.bind();

        GL_CHECK(glActiveTexture(GLenum(INPUT_IMAGE_TEXTURE_UINT)));
        images_[input_image_].bind();
    }

    void geometry_buffer::swap_input_image()
    {
        output_image_++;
        output_image_ %= 2;
        input_image_++;
        input_image_ %= 2;
    }
}
}
