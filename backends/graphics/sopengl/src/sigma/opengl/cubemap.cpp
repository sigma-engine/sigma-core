#include <sigma/opengl/cubemap.hpp>

#include <sigma/opengl/util.hpp>

#include <cmath>

namespace sigma {
namespace opengl {
    cubemap::cubemap(graphics::cubemap_data data)
    {
		int mip_levels = std::min(std::log2(data.right.size.x), std::log2(data.right.size.y));

        // TODO custom internal format
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));
        GL_CHECK(glTexStorage2D(GL_TEXTURE_CUBE_MAP, mip_levels, GL_RGB8, data.right.size.x, data.right.size.y));

        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, data.right.size.x, data.right.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.right.pixels.data()));
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, data.left.size.x, data.left.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.left.pixels.data()));

        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, data.top.size.x, data.top.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.top.pixels.data()));
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, data.bottom.size.x, data.bottom.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.bottom.pixels.data()));

        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, data.back.size.x, data.back.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.back.pixels.data()));
        GL_CHECK(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, data.front.size.x, data.front.size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.front.pixels.data()));

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

    void cubemap::bind()
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));
    }

    std::unique_ptr<graphics::cubemap> cubemap_manager::create(graphics::cubemap_data data)
    {
        return std::make_unique<cubemap>(std::move(data));
    }
}
}
