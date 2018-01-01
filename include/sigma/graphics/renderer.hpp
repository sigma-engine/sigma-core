#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/frustum.hpp>
#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/transform.hpp>
#include <sigma/world.hpp>

#include <glm/vec2.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    class SIGMA_API renderer {
    public:
        struct SIGMA_API settings {
            resource::handle<graphics::post_process_effect> image_based_light_effect;
            resource::handle<graphics::post_process_effect> point_light_effect;
            resource::handle<graphics::post_process_effect> directional_light_effect;
            resource::handle<graphics::post_process_effect> spot_light_effect;
            resource::handle<graphics::post_process_effect> texture_blit_effect;
            resource::handle<graphics::post_process_effect> gamma_conversion;
            resource::handle<graphics::post_process_effect> vignette_effect;
            resource::handle<graphics::technique> shadow_technique;

            template <class Archive>
            void serialize(Archive& ar, const unsigned int version)
            {
                ar& image_based_light_effect;
                ar& point_light_effect;
                ar& directional_light_effect;
                ar& spot_light_effect;
                ar& texture_blit_effect;
                ar& gamma_conversion;
                ar& vignette_effect;
                ar& shadow_technique;
            }
        };

        using render_resource_set = resource_set<sigma::graphics::texture,
            sigma::graphics::cubemap,
            sigma::graphics::shader,
            sigma::graphics::technique,
            sigma::graphics::material,
            sigma::graphics::static_mesh,
            sigma::graphics::post_process_effect>;

        using context_view_type = context_view<render_resource_set>;

        using render_component_set = component_set<sigma::transform,
            sigma::graphics::directional_light,
            sigma::graphics::point_light,
            sigma::graphics::spot_light,
            sigma::graphics::static_mesh_instance>;
        using world_view_type = world_view<render_component_set>;

        renderer(glm::ivec2 size, context_view_type ctx);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(const view_port& viewport, const world_view_type& world) = 0;

    protected:
        context_view_type context_;
        settings settings_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
