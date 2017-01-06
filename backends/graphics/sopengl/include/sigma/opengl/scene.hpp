#ifndef SIGMA_OPENGL_SCENE_HPP
#define SIGMA_OPENGL_SCENE_HPP

#include <sigma/graphics/scene.hpp>

namespace sigma {
namespace opengl {
    class scene : public graphics::scene {
    public:
        virtual std::size_t create_point_light(graphics::point_light light_data, glm::vec3 location, float radius) override;

        virtual void update_point_light(std::size_t light_id, graphics::point_light data, glm::vec3 location, float radius) override;

        virtual void remove_point_light(std::size_t light_id) override;
    };
}
}

#endif // SIGMA_OPENGL_SCENE_HPP
