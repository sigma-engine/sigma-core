#include <sigma/graphics/opengl/texture.hpp>

#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    texture::texture(internal_format format, glm::ivec2 size, graphics::texture_filter minification_filter, graphics::texture_filter magnification_filter, graphics::texture_filter mipmap_filter, const std::vector<unsigned char>& pixels)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (magnification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));

        if (mipmap_filter != graphics::texture_filter::NONE) {
            if (minification_filter == graphics::texture_filter::LINEAR) {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST));
            } else {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
            }

            int mip_levels = calculate_mipmap_levels(size.x, size.y);
            GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, mip_levels, GLenum(format), size.x, size.y));

            GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));
            GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
        } else {
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (minification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));

            GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GLenum(format), size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));
        }
    }

    texture::texture(internal_format format, glm::ivec2 size)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GLenum(format), size.x, size.y));
    }

    texture::texture(graphics::texture_data data)
        : texture(internal_format::RGBA8, data.size, data.minification_filter, data.magnification_filter, data.mipmap_filter, data.pixels)
    {
    }

    texture::~texture()
    {
        glDeleteTextures(1, &object_);
    }

    GLuint texture::get_object() const
    {
        return object_;
    }

    void texture::bind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
    }

    std::unique_ptr<graphics::texture> texture_manager::create(graphics::texture_data data)
    {
        return std::make_unique<texture>(std::move(data));
    }
}
}
