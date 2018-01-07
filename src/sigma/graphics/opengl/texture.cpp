#include <sigma/graphics/opengl/texture.hpp>

#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/texture.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

namespace sigma {
namespace opengl {
    std::pair<GLenum, GLenum> convert_gl(graphics::texture_format fmt)
    {
        switch (fmt) {
        case graphics::texture_format::RGB8:
            return { GL_RGB, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGBA8:
            return { GL_RGBA, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGB32F:
            return { GL_RGB, GL_FLOAT };
        }
        return { GL_RGBA, GL_UNSIGNED_BYTE };
    }

    internal_format convert_internal(graphics::texture_format fmt)
    {
        switch (fmt) {
        case graphics::texture_format::RGB8:
            return internal_format::RGB8;
        case graphics::texture_format::RGBA8:
            return internal_format::RGBA8;
        case graphics::texture_format::RGB32F:
            return internal_format::RGB32F;
        }
        return internal_format::RGBA8;
    }

    texture::texture(internal_format format,
        glm::ivec2 size,
        graphics::texture_filter minification_filter,
        graphics::texture_filter magnification_filter,
        graphics::texture_filter mipmap_filter)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (magnification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));

        int mip_levels = 1;
        if (mipmap_filter != graphics::texture_filter::NONE) {
            // TODO this breaks DRY, it is also in graphics::texture.
            mip_levels = 1 + std::floor(std::log2(std::max(size.x, size.y)));

            if (minification_filter == graphics::texture_filter::LINEAR) {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST));
            } else {
                GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (mipmap_filter == graphics::texture_filter::LINEAR) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
            }
        } else {
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (minification_filter == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST));
        }

        GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, mip_levels, GLenum(format), size.x, size.y));
    }

    texture::texture(const graphics::texture& data)
        : texture(convert_internal(data.format()),
              data.size(),
              data.minification_filter(),
              data.magnification_filter(),
              data.mipmap_filter())
    {
        auto gl_format = convert_gl(data.format());
        for (std::size_t i = 0; i < data.stored_mipmap_count(); ++i) {
            GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, data.size().x >> i, data.size().y >> i, gl_format.first, gl_format.second, data.data(i)));
        }

        if (data.mipmap_filter() != graphics::texture_filter::NONE && data.stored_mipmap_count() != data.total_mipmap_count()) {
            // TODO this generates from level 0 to N
            // make it start at the last level loaded
            GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
        }
    }

    texture::texture(texture&& other)
    {
        this->object_ = other.object_;
        other.object_ = 0;
    }

    texture& texture::operator=(texture&& other)
    {
        this->object_ = other.object_;
        other.object_ = 0;
        return *this;
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

    void texture::generate_mipmaps()
    {
        GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    }
}
}
