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
        graphics::texture* textures[6];
        for (unsigned int i = 0; i < 6; ++i)
            textures[i] = texture_cache.acquire(data.faces[i]);

        // TODO don't generate mipmaps if mipmaps are disabled for this cubemap.
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));
        GL_CHECK(glTexStorage2D(GL_TEXTURE_CUBE_MAP, textures[0]->total_mipmap_count(), (GLenum)convert_internal(textures[0]->format()), textures[0]->size().x, textures[0]->size().y));

        for (unsigned int i = 0; i < 6; ++i) {
            auto txt = textures[i];
            assert(txt->size() == textures[0]->size());
            assert(txt->format() == textures[0]->format());
            assert(txt->stored_mipmap_count() == textures[0]->stored_mipmap_count());
            auto gl_face = convert_gl(static_cast<graphics::cubemap::face>(i));
            auto gl_format = convert_gl(txt->format());
            for (std::size_t j = 0; j < txt->stored_mipmap_count(); ++j) {
                std::size_t size_x = std::max(1, txt->size().x >> j);
                std::size_t size_y = std::max(1, txt->size().y >> j);
                GL_CHECK(glTexSubImage2D(gl_face, j, 0, 0, size_x, size_y, gl_format.first, gl_format.second, txt->data(j)));
            }
        }

        // TODO get texture parameters from the cubemap.
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

        if (textures[0]->stored_mipmap_count() != textures[0]->total_mipmap_count()) {
            GL_CHECK(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        }
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
