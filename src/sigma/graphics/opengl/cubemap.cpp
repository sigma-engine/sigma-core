#include <sigma/graphics/opengl/cubemap.hpp>

#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    cubemap::cubemap(graphics::cubemap_data data)
    {
        // TODO custom internal format
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));

        int mip_levels = calculate_mipmap_levels(data.right.size.x, data.right.size.y);
        GL_CHECK(glTexStorage2D(GL_TEXTURE_CUBE_MAP, mip_levels, (GLenum)convert_internal(data.right.format), data.right.size.x, data.right.size.y));

        // X-axis
        auto gl_format = convert_gl(data.right.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, data.right.size.x, data.right.size.y, gl_format.first, gl_format.second, data.right.data.data()));

        gl_format = convert_gl(data.left.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, data.left.size.x, data.left.size.y, gl_format.first, gl_format.second, data.left.data.data()));

        // Y-axis
        gl_format = convert_gl(data.top.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, data.top.size.x, data.top.size.y, gl_format.first, gl_format.second, data.top.data.data()));

        gl_format = convert_gl(data.bottom.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, data.bottom.size.x, data.bottom.size.y, gl_format.first, gl_format.second, data.bottom.data.data()));

        // Z-axis
        gl_format = convert_gl(data.back.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, data.back.size.x, data.back.size.y, gl_format.first, gl_format.second, data.back.data.data()));

        gl_format = convert_gl(data.front.format);
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, data.front.size.x, data.front.size.y, gl_format.first, gl_format.second, data.front.data.data()));

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

    std::unique_ptr<graphics::cubemap> cubemap_manager::create(graphics::cubemap_data data)
    {
        return std::make_unique<cubemap>(std::move(data));
    }
}
}
