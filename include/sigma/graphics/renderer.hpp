#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>

#include <sigma/component.hpp>
#include <sigma/context.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <sigma/world.hpp>

#include <glm/vec2.hpp>

#include <memory>

namespace sigma {
namespace graphics {

    class renderer {
    public:
        using render_component_set = sigma::component_set<sigma::transform,
            sigma::graphics::directional_light,
            sigma::graphics::point_light,
            sigma::graphics::spot_light,
            sigma::graphics::static_mesh_instance>;
        using world_view_type = world_view<render_component_set>;

        renderer(glm::ivec2 size, std::shared_ptr<sigma::context> ctx);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(const world_view_type& world) = 0;

    protected:
        std::shared_ptr<sigma::context> context_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
