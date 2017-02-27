#include <sigma/transform.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace sigma {

transform::transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    : position_(position)
    , rotation_(rotation)
    , scale_(scale)
{
    dirty_ = true;
}

glm::vec3 transform::position() const
{
    return position_;
}

void transform::set_position(const glm::vec3& position)
{
    dirty_ = true;
    position_ = position;
}

glm::quat transform::rotation() const
{
    return rotation_;
}

void transform::set_rotation(const glm::quat& rotation)
{
    dirty_ = true;
    rotation_ = rotation;
}

glm::vec3 transform::scale() const
{
    return scale_;
}

void transform::set_scale(const glm::vec3& scale)
{
    dirty_ = true;
    scale_ = scale;
}

bool transform::dirty() const
{
    return dirty_;
}

void transform::set_dirty(bool dirty)
{
    dirty_ = dirty;
}

const glm::mat4& transform::matrix()
{
    if (dirty_) {
        matrix_ = glm::translate(glm::mat4(1), position_) * glm::mat4_cast(rotation_) * glm::scale(glm::mat4(1), glm::vec3(scale_));
        dirty_ = false;
    }
    return matrix_;
}
}
