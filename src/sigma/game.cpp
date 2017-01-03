#include <sigma/game.hpp>

#include <sigma/context.hpp>
#include <sigma/util/json_conversion.hpp>

#include <json/json.h>

#include <boost/algorithm/string/predicate.hpp>

#include <fstream>

namespace sigma {
game::game(graphics::renderer *renderer)
    : renderer_(renderer)
{
}



void game::load(boost::filesystem::path file_path)
{
    // TODO load
    std::ifstream file{ file_path.string(), std::ios::in };
    Json::Value scene_data;
    file >> scene_data;

    auto entity_names = scene_data.getMemberNames();
    std::for_each(entity_names.begin(), entity_names.end(), [&](const std::string& entity_name) {
        const auto& scene_entity = scene_data[entity_name];
        const auto& entity_component_types = scene_entity.getMemberNames();

        auto e = entities.create();
        std::for_each(entity_component_types.begin(), entity_component_types.end(), [&](const std::string& component_type) {
            const auto& component = scene_entity[component_type];
            if (component_type == "sigma::transform") {
                const auto& component_values = component.getMemberNames();
                auto& txform = transforms.add(e);
                std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                    const auto& value = component[value_name];
                    if (value_name == "position") {
                        glm::vec3 position;
                        json::from_json(value, position);
                        txform.set_position(position);
                    }
                    else if (value_name == "scale") {
                        glm::vec3 scale;
                        json::from_json(value,scale);
                        txform.set_scale(scale);
                    }
                    else if (value_name == "rotation") {
                        glm::quat rotation;
                        json::from_json(value, rotation);
                        txform.set_rotation(rotation);
                    }
                });
            } else if (component_type == "sigma::graphics::directional_light") {
                const auto& component_values = component.getMemberNames();
                auto& light = directional_lights.add(e);
                std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                    const auto& value = component[value_name];
                    if (value_name == "color")
                        json::from_json(value, light.color);
                    else if (value_name == "intensity")
                        json::from_json(value, light.intensity);
                });
            } else if (component_type == "sigma::graphics::point_light") {
                const auto& component_values = component.getMemberNames();
                auto& light = point_lights.add(e);
                std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                    const auto& value = component[value_name];
                    if (value_name == "color")
                        json::from_json(value, light.color);
                    else if (value_name == "intensity")
                        json::from_json(value, light.intensity);
                });
            } else if (component_type == "sigma::graphics::static_mesh") {
                auto resource_name = component.asString();
                if (!boost::starts_with(resource_name, "static_mesh://"))
                    resource_name = "static_mesh://" + resource_name;
                static_mesh_instances.add(e, renderer_->static_meshes().get(resource_name));
            }
        });
    });
}
}
