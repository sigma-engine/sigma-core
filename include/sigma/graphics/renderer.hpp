#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>

#include <sigma/context.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <entt/entt.hpp>

#include <glm/vec2.hpp>

#include <memory>

namespace sigma {
namespace graphics {

    class renderer {
    public:
        renderer(glm::ivec2 size, std::shared_ptr<sigma::context> ctx);

        virtual ~renderer() = default;

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(const entt::registry<>& registry) = 0;

    protected:
        std::shared_ptr<sigma::context> context_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
