#include <sigma/graphics/opengl/cubemap_manager.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/opengl/texture_manager.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    GLenum convert(graphics::cubemap::face face)
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

    cubemap_manager::cubemap_manager(resource::cache<graphics::texture>& texture_cache, resource::cache<graphics::cubemap>& cubemap_cache)
        : texture_cache_(texture_cache)
        , cubemap_cache_(cubemap_cache)
    {
    }

    cubemap_manager::~cubemap_manager()
    {
        glDeleteTextures(cubemaps_.size(), cubemaps_.data());
    }

    std::pair<graphics::cubemap*, GLuint> cubemap_manager::acquire(const resource::handle<graphics::cubemap>& hndl)
    {
        assert(hndl.is_valid());
        if (hndl.index >= cubemaps_.size())
            cubemaps_.resize(hndl.index + 1, 0);

        auto data = cubemap_cache_.acquire(hndl);
        if (cubemaps_[hndl.index] == 0) {
            graphics::texture* textures[6];
            for (unsigned int i = 0; i < 6; ++i)
                textures[i] = texture_cache_.acquire(data->faces[i]);

            auto size = textures[0]->size();
            auto format = convert(textures[0]->format());

            // TODO don't generate mipmaps if mipmaps are disabled for this cubemap.
            glGenTextures(1, &cubemaps_[hndl.index]);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps_[hndl.index]);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, textures[0]->total_mipmap_count(), std::get<0>(format), size.x, size.y);

            for (unsigned int i = 0; i < 6; ++i) {
                auto txt = textures[i];
                assert(txt->size() == textures[0]->size());
                assert(txt->format() == textures[0]->format());
                assert(txt->stored_mipmap_count() == textures[0]->stored_mipmap_count());
                auto face = convert(static_cast<graphics::cubemap::face>(i));
                for (std::size_t j = 0; j < txt->stored_mipmap_count(); ++j) {
                    std::size_t size_x = std::max(1, size.x >> j);
                    std::size_t size_y = std::max(1, size.y >> j);
                    glTexSubImage2D(face, j, 0, 0, size_x, size_y, std::get<1>(format), std::get<2>(format), txt->data(j));
                }
            }

            // TODO get texture parameters from the cubemap.
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            if (textures[0]->stored_mipmap_count() != textures[0]->total_mipmap_count()) {
                // TODO this generates from level 0 to N
                // make it start at the last level loaded
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            }
        }

        return { data, cubemaps_[hndl.index] };
    }
}
}
