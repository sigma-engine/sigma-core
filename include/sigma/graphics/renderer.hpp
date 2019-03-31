#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <entt/entt.hpp>
#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/frustum.hpp>

#include <glm/vec2.hpp>

#include <memory>

namespace sigma {
namespace graphics {

    struct view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    class render_queue;

    class renderer {
    public:
        renderer(glm::ivec2 size, std::shared_ptr<sigma::context> ctx);

        virtual ~renderer() = default;

        virtual render_queue* queue() = 0;

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render() = 0;

    protected:
        std::shared_ptr<sigma::context> context_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
