#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>

#include <sigma/frustum.hpp>

#include <glm/vec2.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    class SIGMA_API renderer {
    public:
        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
