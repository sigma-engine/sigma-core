#include <sigma/graphics/opengl/cubemap.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    cubemap::cubemap(resource::cache<graphics::texture>& texture_cache, const graphics::cubemap& data)
    {
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));

        auto right_texture = texture_cache.acquire(data.right);
        auto left_texture = texture_cache.acquire(data.left);
        auto top_texture = texture_cache.acquire(data.top);
        auto bottom_texture = texture_cache.acquire(data.bottom);
        auto back_texture = texture_cache.acquire(data.back);
        auto front_texture = texture_cache.acquire(data.front);

        int mip_levels = calculate_mipmap_levels(right_texture->size.x, right_texture->size.y);
        GL_CHECK(glTexStorage2D(GL_TEXTURE_CUBE_MAP, mip_levels, (GLenum)convert_internal(right_texture->format), right_texture->size.x, right_texture->size.y));

        // X-axis
        auto gl_format = convert_gl(right_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, right_texture->size.x, right_texture->size.y, gl_format.first, gl_format.second, right_texture->data.data()));

        gl_format = convert_gl(left_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, left_texture->size.x, left_texture->size.y, gl_format.first, gl_format.second, left_texture->data.data()));

        // Y-axis
        gl_format = convert_gl(top_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, top_texture->size.x, top_texture->size.y, gl_format.first, gl_format.second, top_texture->data.data()));

        gl_format = convert_gl(bottom_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, bottom_texture->size.x, bottom_texture->size.y, gl_format.first, gl_format.second, bottom_texture->data.data()));

        // Z-axis
        gl_format = convert_gl(back_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, front_texture->size.x, front_texture->size.y, gl_format.first, gl_format.second, front_texture->data.data()));

        gl_format = convert_gl(front_texture->format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, back_texture->size.x, back_texture->size.y, gl_format.first, gl_format.second, back_texture->data.data()));

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
