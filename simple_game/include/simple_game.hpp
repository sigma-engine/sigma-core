#ifndef SIMPLE_GAME_HPP
#define SIMPLE_GAME_HPP

#include <grid_component.hpp>

#include <sigma/tools/json_conversion.hpp>
#include <sigma/blueprint.hpp>
#include <sigma/context.hpp>
#include <sigma/game.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>

#include <glm/vec3.hpp>

using simple_component_set = sigma::component_set<sigma::transform,
    sigma::graphics::directional_light,
    sigma::graphics::point_light,
    sigma::graphics::spot_light,
    sigma::graphics::static_mesh_instance,
    grid_component>;

using simple_blueprint = sigma::blueprint<simple_component_set>;

struct simple_level_settings {
    static constexpr const char* GROUP = "level";

    sigma::resource::handle<simple_blueprint> current_level_blueprint;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(current_level_blueprint);
    }

    void load_settings(std::shared_ptr<sigma::context> context, const Json::Value &settings)
    {
        sigma::json::from_json(context, settings["current_level_blueprint"], current_level_blueprint);
    }
};

class simple_game : public sigma::game {
public:
    simple_game(std::shared_ptr<sigma::context> ctx);

    virtual void update(std::chrono::duration<float> dt) override;
};

#endif // SIMPLE_GAME_HPP
