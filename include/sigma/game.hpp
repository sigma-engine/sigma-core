#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

#include <boost/algorithm/string/predicate.hpp>

#include <chrono>
#include <fstream>

namespace sigma {

template <class World>
class game {
public:
    game(graphics::renderer* renderer)
        : renderer_(renderer)
    {
    }

    virtual ~game() = default;

    World& world()
    {
        return world_;
    }

    void load(boost::filesystem::path file_path)
    {
        std::ifstream file{ file_path.string(), std::ios::in };
        Json::Value scene_data;
        file >> scene_data;

        auto entity_names = scene_data.getMemberNames();
        std::for_each(entity_names.begin(), entity_names.end(), [&](const std::string& entity_name) {
            const auto& scene_entity = scene_data[entity_name];
            const auto& entity_component_types = scene_entity.getMemberNames();

            auto e = world_.create();
            std::for_each(entity_component_types.begin(), entity_component_types.end(), [&](const std::string& component_type) {
                const auto& component = scene_entity[component_type];
                if (component_type == "sigma::transform") {
                    const auto& component_values = component.getMemberNames();

                    glm::vec3 position;
                    glm::quat rotation;
                    glm::vec3 scale{ 1 };
                    std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                        const auto& value = component[value_name];
                        if (value_name == "position") {
                            json::from_json(value, position);
                        } else if (value_name == "scale") {
                            json::from_json(value, scale);
                        } else if (value_name == "rotation") {
                            json::from_json(value, rotation);
                        }
                    });
                    world_.template add<transform>(e, position, rotation, scale);
                } else if (component_type == "sigma::graphics::directional_light") {
                    const auto& component_values = component.getMemberNames();
                    glm::vec3 color{ 1 };
                    float intensity{ 1 };
                    std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                        const auto& value = component[value_name];
                        if (value_name == "color")
                            json::from_json(value, color);
                        else if (value_name == "intensity")
                            json::from_json(value, intensity);
                    });
                    world_.template add<graphics::directional_light>(e, color, intensity);
                } else if (component_type == "sigma::graphics::point_light") {
                    const auto& component_values = component.getMemberNames();
                    glm::vec3 color{ 1 };
                    float intensity{ 1 };
                    std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                        const auto& value = component[value_name];
                        if (value_name == "color")
                            json::from_json(value, color);
                        else if (value_name == "intensity")
                            json::from_json(value, intensity);
                    });
                    world_.template add<graphics::point_light>(e, color, intensity);
                } else if (component_type == "sigma::graphics::static_mesh") {
                    auto resource_name = component.asString();
                    if (!boost::starts_with(resource_name, "static_mesh://"))
                        resource_name = "static_mesh://" + resource_name;
                    world_.template add<graphics::static_mesh_instance>(e, renderer_->static_meshes().get(resource_name));
                }
            });
        });
    }

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    World world_;
    graphics::renderer* renderer_;
};
}

#endif // SIGMA_GAME_HPP
