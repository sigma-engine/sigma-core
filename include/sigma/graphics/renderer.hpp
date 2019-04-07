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

#include <entt/entt.hpp>

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
            static constexpr const char* GROUP = "render";

            BOOST_HANA_DEFINE_STRUCT(settings,
                (bool, enable_image_based_lighting),
                (bool, enable_debug_rendering),
                (resource::handle<graphics::post_process_effect>, image_based_light_effect),
                (resource::handle<graphics::post_process_effect>, point_light_effect),
                (resource::handle<graphics::post_process_effect>, directional_light_effect),
                (resource::handle<graphics::post_process_effect>, spot_light_effect),
                (resource::handle<graphics::post_process_effect>, texture_blit_effect),
                (resource::handle<graphics::post_process_effect>, gamma_conversion),
                (resource::handle<graphics::post_process_effect>, vignette_effect),
                (resource::handle<graphics::technique>, shadow_technique));

            template <class Archive>
            void serialize(Archive& ar)
            {
                ar(enable_image_based_lighting,
                enable_debug_rendering,
                image_based_light_effect,
                point_light_effect,
                directional_light_effect,
                spot_light_effect,
                texture_blit_effect,
                gamma_conversion,
                vignette_effect,
                shadow_technique);
            }
        };

        using render_resource_set = resource_set<sigma::graphics::texture,
            sigma::graphics::cubemap,
            sigma::graphics::shader,
            sigma::graphics::technique,
            sigma::graphics::material,
            sigma::graphics::static_mesh,
            sigma::graphics::post_process_effect>;

        using render_settings_set = settings_set<settings>;

        using context_view_type = context_view<render_resource_set, render_settings_set>;

        using render_component_set = component_set<sigma::transform,
            sigma::graphics::directional_light,
            sigma::graphics::point_light,
            sigma::graphics::spot_light,
            sigma::graphics::static_mesh_instance>;

        renderer(glm::ivec2 size, context_view_type ctx);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(const view_port& viewport, const entt::registry<>& registry) = 0;

    protected:
        context_view_type context_;
        settings& settings_;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP
