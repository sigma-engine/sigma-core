#ifndef SIGMAFIVE_ENGINE_GRAPHICS_CONTEXT_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_CONTEXT_HPP

#include <sigmafive/config.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace sigmafive {
namespace graphics {
    class SIGMAFIVE_API context {
    public:
        context();

        virtual ~context();

        virtual void initialize() = 0;

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(glm::mat4 projection, glm::mat4 view) = 0;

    private:
    };
}
}

#endif //SIGMAFIVE_ENGINE_GRAPHICS_CONTEXT_HPP
