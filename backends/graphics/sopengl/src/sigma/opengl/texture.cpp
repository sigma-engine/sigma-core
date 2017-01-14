#include <sigma/opengl/texture.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    texture::texture(internal_format format, glm::ivec2 size, const std::vector<unsigned char>& pixels)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GLenum(format), size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));
    }

    texture::texture(internal_format format, glm::ivec2 size)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexStorage2D(GL_TEXTURE_2D, 1, GLenum(format), size.x, size.y));
    }

    texture::texture(graphics::texture_data data)
        : texture(internal_format::RGBA8, data.size, data.pixels)
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

    void texture::bind()
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, object_));
    }

    std::unique_ptr<graphics::texture> texture_manager::create(graphics::texture_data data)
    {
        return std::make_unique<texture>(std::move(data));
    }
}
}
