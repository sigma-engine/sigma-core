#include <sigma/opengl/geometry_buffer.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    geometry_buffer::geometry_buffer(glm::ivec2 size)
        : frame_buffer(size)
        , position_texture_(internal_format::RGB32F, size)
        , diffuse_texture_(internal_format::RGB32F, size)
        , normal_texture_(internal_format::RGB32F, size)
        , depth_texture_(internal_format::DEPTH_COMPONENT32F, size)
    {
        attach(frame_buffer::attachment::COLOR0, position_texture_);
        attach(frame_buffer::attachment::COLOR1, diffuse_texture_);
        attach(frame_buffer::attachment::COLOR2, normal_texture_);
        attach(frame_buffer::attachment::DEPTH, depth_texture_);
        draw_buffers({ frame_buffer::attachment::COLOR0,
            frame_buffer::attachment::COLOR1,
            frame_buffer::attachment::COLOR2 });
    }

    void geometry_buffer::bind_textures()
    {
        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        position_texture_.bind();

        GL_CHECK(glActiveTexture(GL_TEXTURE1));
        diffuse_texture_.bind();

        GL_CHECK(glActiveTexture(GL_TEXTURE2));
        normal_texture_.bind();
    }
}
}
