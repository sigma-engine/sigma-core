#include <sigma/graphics/opengl/texture_manager.hpp>

#include <sigma/graphics/opengl/util.hpp>
#include <sigma/graphics/texture.hpp>

#include <glad/glad.h>

#include <glm/vec2.hpp>

#include <cassert>

namespace sigma {
namespace opengl {
    std::tuple<GLenum, GLenum, GLenum> convert(graphics::texture_format fmt)
    {
        switch (fmt) {
        case graphics::texture_format::RGB8:
            return { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGBA8:
            return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE };
        case graphics::texture_format::RGB32F:
            return { GL_RGB32F, GL_RGB, GL_FLOAT };
        }
        return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE };
    }

    texture_manager::texture_manager(resource::cache<graphics::texture>& texture_cache)
        : texture_cache_(texture_cache)
    {
    }

    texture_manager::~texture_manager()
    {
        glDeleteTextures(textures_.size(), textures_.data());
    }

    std::pair<graphics::texture*, GLuint> texture_manager::acquire(const resource::handle<graphics::texture>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= textures_.size())
            textures_.resize(hndl.index + 1, 0);

        auto data = texture_cache_.acquire(hndl);

        if (textures_[hndl.index] == 0) {
            auto size = data->size();
            auto format = convert(data->format());

            glGenTextures(1, &textures_[hndl.index]);
            glBindTexture(GL_TEXTURE_2D, textures_[hndl.index]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (data->magnification_filter() == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST);

            int mip_levels = 1;
            if (data->mipmap_filter() != graphics::texture_filter::NONE) {
                mip_levels = 1 + std::floor(std::log2(std::max(size.x, size.y)));

                if (data->minification_filter() == graphics::texture_filter::LINEAR) {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (data->mipmap_filter() == graphics::texture_filter::LINEAR) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
                } else {
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (data->mipmap_filter() == graphics::texture_filter::LINEAR) ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
                }
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (data->minification_filter() == graphics::texture_filter::LINEAR) ? GL_LINEAR : GL_NEAREST);
            }

            glTexStorage2D(GL_TEXTURE_2D, mip_levels, std::get<0>(format), size.x, size.y);

            for (std::size_t i = 0; i < data->stored_mipmap_count(); ++i) {
                std::size_t size_x = std::max(1, size.x >> i);
                std::size_t size_y = std::max(1, size.y >> i);
                glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, size_x, size_y, std::get<1>(format), std::get<2>(format), data->data(i));
            }

            if (data->mipmap_filter() != graphics::texture_filter::NONE && data->stored_mipmap_count() != data->total_mipmap_count()) {
                // TODO this generates from level 0 to N
                // make it start at the last level loaded
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }

        return { data, textures_[hndl.index] };
    }
}
}
