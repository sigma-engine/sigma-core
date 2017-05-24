#include <sigma/graphics/opengl/shadow_buffer.hpp>

#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

namespace sigma {
namespace opengl {
    shadow_buffer::shadow_buffer(glm::ivec2 resolution, std::size_t count)
        : frame_buffer(resolution)
        , depth_buffer_(internal_format::DEPTH_COMPONENT16, resolution)
    {
        for (std::size_t i = 0; i < count; ++i) {
            shadow_maps_.emplace_back(internal_format::RG32F, resolution, graphics::texture_filter::LINEAR, graphics::texture_filter::LINEAR, graphics::texture_filter::LINEAR);

            // TODO move this into texture
            shadow_maps_[i].bind();
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        }

        attach(frame_buffer::attachment::DEPTH, depth_buffer_);
    }

    std::size_t shadow_buffer::count() const
    {
        return shadow_maps_.size();
    }

    void shadow_buffer::bind_for_shadow_write(int index)
    {
        attach(frame_buffer::attachment::COLOR0, shadow_maps_[index]);
        draw_buffers(frame_buffer::attachment::COLOR0);
        GL_CHECK(glViewport(0, 0, size_.x, size_.y));
    }

    void shadow_buffer::bind_for_shadow_read(texture_unit unit)
    {
        for (std::size_t i = 0; i < shadow_maps_.size(); ++i) {
            GL_CHECK(glActiveTexture(GLenum(unit) + i));
            shadow_maps_[i].bind();
            shadow_maps_[i].generate_mipmaps();
        }
    }
}
}
