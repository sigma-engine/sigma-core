#ifndef SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP
#define SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/cubemap.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    GLenum convert(graphics::cubemap::face face);

    class cubemap_manager {
    public:
        cubemap_manager(resource::cache<graphics::texture>& texture_cache, resource::cache<graphics::cubemap>& cubemap_cache);

        cubemap_manager(cubemap_manager&&) = default;

        cubemap_manager& operator=(cubemap_manager&&) = default;

        ~cubemap_manager();

        std::pair<graphics::cubemap*, GLuint> acquire(const resource::handle<graphics::cubemap>& hndl);

    private:
        cubemap_manager(const cubemap_manager&) = delete;
        cubemap_manager& operator=(const cubemap_manager&) = delete;

        resource::cache<graphics::texture>& texture_cache_;
        resource::cache<graphics::cubemap>& cubemap_cache_;
        std::vector<GLuint> cubemaps_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP
