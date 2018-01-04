#include <sigma/graphics/opengl/cubemap.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    GLenum convert_gl(graphics::cubemap::face face)
    {
        switch (face) {
        case graphics::cubemap::face::POSITIVE_X:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case graphics::cubemap::face::NEGATIVE_X:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case graphics::cubemap::face::POSITIVE_Y:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case graphics::cubemap::face::NEGATIVE_Y:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        case graphics::cubemap::face::POSITIVE_Z:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case graphics::cubemap::face::NEGATIVE_Z:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        }
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }

    cubemap::cubemap(resource::cache<graphics::texture>& texture_cache, const graphics::cubemap& data)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));

        graphics::texture_format format;
        glm::ivec2 size;
        for (unsigned int i = 0; i < 6; ++i) {
            auto texture = texture_cache.acquire(data.faces[i]);
            if (i == 0) {
                format = texture->format;
                size = texture->size;
                int mip_levels = calculate_mipmap_levels(texture->size.x, texture->size.y);
                GL_CHECK(glTexStorage2D(GL_TEXTURE_CUBE_MAP, mip_levels, (GLenum)convert_internal(texture->format), texture->size.x, texture->size.y));
            }

            // TODO clean this code up.
            assert(format == texture->format);
            assert(size == texture->size);
            auto gl_format = convert_gl(format);
            GL_CHECK(glTexSubImage2D(convert_gl(static_cast<graphics::cubemap::face>(i)), 0, 0, 0, size.x, size.y, gl_format.first, gl_format.second, texture->data.data()));
        }

        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        GL_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
    }

    cubemap::~cubemap()
    {
        glDeleteTextures(1, &object_);
    }

    GLuint cubemap::get_object() const
    {
        return object_;
    }

    void cubemap::bind() const
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));
    }
}
}
