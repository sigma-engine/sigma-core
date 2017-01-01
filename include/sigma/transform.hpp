#ifndef SIGMA_TRANSFORM_HPP
#define SIGMA_TRANSFORM_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sigma {

struct SIGMA_API transform {
public:

    glm::vec3 position() const;

    void set_position(const glm::vec3 &position);

    glm::quat rotation() const;

    void set_rotation(const glm::quat &rotation);

    glm::vec3 scale() const;

    void set_scale(const glm::vec3 &scale);

    bool dirty() const;

    void set_dirty(bool dirty=true);

    const glm::mat4 &matrix();

private:
    glm::vec3 position_;
    glm::quat rotation_;
    glm::vec3 scale_{ 1.0f };
    bool dirty_;
    glm::mat4 matrix_;
};

using transform_manager = basic_component_manager<transform>;



















}

#endif // SIGMA_TRANSFORM_HPP
