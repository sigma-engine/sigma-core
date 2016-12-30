#include <sigma/game.hpp>

#include <sigma/context.hpp>

#include <json/json.h>

#include <boost/algorithm/string/predicate.hpp>

namespace sigma {
game::game(context* ctx)
    : ctx(ctx)
    , textures(ctx->textures())
    , shaders(ctx->shaders())
    , effects(ctx->effects())
    , materials(ctx->materials())
    , static_meshes(ctx->static_meshes())
    , static_mesh_instances(static_meshes)
{
}

bool to_float(const Json::Value& value, float& output)
{
    if (value.isConvertibleTo(Json::realValue)) {
        output = float(value.asDouble());
        return true;
    }
    return false;
}

bool to_vec3(const Json::Value& value, glm::vec3& output)
{
    // TODO support x,y,z
    return value.isArray() && value.size() == 3 && to_float(value[0], output.x) && to_float(value[1], output.y) && to_float(value[2], output.z);
}

bool to_quat(const Json::Value& value, glm::quat& output)
{
    glm::vec3 e;
    if (to_vec3(value, e)) {
        output = glm::quat{ glm::radians(e) };
        return true;
    }
    // TODO support x,y,z,w
    return false;
}

void game::load(boost::filesystem::path file_path)
{
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
                    if (value_name == "position")
                        to_vec3(value, txform.position);
                    else if (value_name == "scale")
                        to_vec3(value, txform.scale);
                    else if (value_name == "rotation")
                        to_quat(value, txform.rotation);
                });
            } else if (component_type == "sigma::graphics::directional_light") {
                const auto& component_values = component.getMemberNames();
                auto& light = directional_lights.add(e);
                std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                    const auto& value = component[value_name];
                    if (value_name == "color")
                        to_vec3(value, light.color);
                    else if (value_name == "intensity")
                        to_float(value, light.intensity);
                });
            } else if (component_type == "sigma::graphics::point_light") {
                const auto& component_values = component.getMemberNames();
                auto& light = point_lights.add(e);
                std::for_each(component_values.begin(), component_values.end(), [&](const std::string& value_name) {
                    const auto& value = component[value_name];
                    if (value_name == "color")
                        to_vec3(value, light.color);
                    else if (value_name == "intensity")
                        to_float(value, light.intensity);
                });
            } else if (component_type == "sigma::graphics::static_mesh") {
                auto resource_name = component.asString();
                if (!boost::starts_with(resource_name, "static_mesh://"))
                    resource_name = "static_mesh://" + resource_name;
                static_mesh_instances.add(e, resource_name);
            }
        });
    });
}
}
