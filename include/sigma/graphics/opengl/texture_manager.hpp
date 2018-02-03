#ifndef SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP
#define SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP

#include <sigma/graphics/texture.hpp>

#include <sigma/config.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <vector>

namespace sigma {
namespace opengl {
    std::tuple<GLenum, GLenum, GLenum> convert(graphics::texture_format fmt);

    class texture_manager {
    public:
        texture_manager(resource::cache<graphics::texture>& texture_cache);

        texture_manager(texture_manager&&) = default;

        texture_manager& operator=(texture_manager&&) = default;

        ~texture_manager();

        std::pair<graphics::texture*, GLuint> acquire(const resource::handle<graphics::texture>& hndl);

    private:
        texture_manager(const texture_manager&) = delete;
        texture_manager& operator=(const texture_manager&) = delete;

        resource::cache<graphics::texture>& texture_cache_;
        std::vector<GLuint> textures_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_TEXTURE_HPP
