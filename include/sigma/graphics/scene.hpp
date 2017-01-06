#ifndef SIGMA_ENGINE_GRAPHICS_SCENE_HPP
#define SIGMA_ENGINE_GRAPHICS_SCENE_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/point_light.hpp>

namespace sigma {
namespace graphics {

    class SIGMA_API scene {
    public:
        scene() = default;

        scene(scene&&) = default;

        scene& operator=(scene&&) = default;

        virtual ~scene() = default;

        virtual std::size_t create_point_light(graphics::point_light light_data, glm::vec3 location, float radius) = 0;

        virtual void update_point_light(std::size_t light_id, graphics::point_light data, glm::vec3 location, float radius) = 0;

        virtual void remove_point_light(std::size_t light_id) = 0;

    private:
        scene(const scene&) = delete;
        scene& operator=(const scene&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_GRAPHICS_SCENE_HPP
