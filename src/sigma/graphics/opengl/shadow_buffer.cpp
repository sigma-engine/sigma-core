#include <sigma/graphics/opengl/shadow_buffer.hpp>

#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

namespace sigma {
namespace opengl {
    shadow_buffer::shadow_buffer(glm::ivec2 size)
        : frame_buffer(size)
        , shadow_map_(internal_format::RG32F, size, graphics::texture_filter::LINEAR, graphics::texture_filter::LINEAR, graphics::texture_filter::LINEAR)
        , depth_buffer_(internal_format::DEPTH_COMPONENT16, size)
    {
        // TODO move this into texture
        shadow_map_.bind();
        GLfloat border_color[] = { -1.0, -1.0, 0.0, 0.0 };
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GL_CHECK(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color));

        attach(frame_buffer::attachment::COLOR0, shadow_map_);
        attach(frame_buffer::attachment::DEPTH, depth_buffer_);
    }

    void shadow_buffer::bind_for_shadow_write()
    {
        draw_buffers(frame_buffer::attachment::COLOR0);
        GL_CHECK(glViewport(0, 0, size_.x, size_.y));
    }

    void shadow_buffer::bind_for_shadow_read(texture_unit unit)
    {
        GL_CHECK(glActiveTexture(GLenum(unit)));
        shadow_map_.bind();
        shadow_map_.generate_mipmaps();
    }
}
}
