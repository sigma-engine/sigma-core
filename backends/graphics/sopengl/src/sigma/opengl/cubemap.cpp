#include <sigma/opengl/cubemap.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    cubemap::cubemap(graphics::cubemap_data data)
    {
        // TODO custom internal format
        GL_CHECK(glGenTextures(1, &object_));
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, object_));

        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, data.right.size.x, data.right.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.right.pixels.data()));
        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, data.left.size.x, data.left.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.left.pixels.data()));

        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, data.top.size.x, data.top.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.top.pixels.data()));
        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, data.bottom.size.x, data.bottom.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.bottom.pixels.data()));

        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, data.back.size.x, data.back.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.back.pixels.data()));
        GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, data.front.size.x, data.front.size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.front.pixels.data()));

        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
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

    std::unique_ptr<graphics::cubemap> cubemap_manager::load(graphics::cubemap_data data, boost::archive::binary_iarchive& ia)
    {
        return std::make_unique<cubemap>(std::move(data));
    }
}
}
