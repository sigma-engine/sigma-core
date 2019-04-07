#ifndef SIGMA_GRAPHICS_POINT_LIGHT_HPP
#define SIGMA_GRAPHICS_POINT_LIGHT_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/reflect.hpp>

#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    struct R_EXPORT() point_light {
        glm::vec3 color;
        float intensity;

        point_light(const glm::vec3& color = { 1.0f, 1.0f, 1.0f }, float intensity = 1.0f)
            : color{ color }
            , intensity{ intensity }
        {
        }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(color, intensity);
        }
    };
}

namespace json {
    static bool from_json(std::shared_ptr<context> ctx, const Json::Value& source, graphics::point_light& output)
    {
        if (source.isMember("color"))
            from_json(ctx, source["color"], output.color);
        
        if (source.isMember("intensity"))
            from_json(ctx, source["intensity"], output.intensity);
        return true;
    }   
}
}

REGISTER_COMPONENT(sigma::graphics::point_light)

#endif // SIGMA_GRAPHICS_POINT_LIGHT_HPP
