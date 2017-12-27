#include <sigma/trackball_controller.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace sigma {
trackball_controller::trackball_controller()
    : rotating_(false)
    , panning_(false)
{
}

glm::mat4 trackball_controller::matrix() const
{
    glm::mat4 mat;
    return glm::inverse(glm::translate(mat, position_) * glm::mat4_cast(rotation_));
}

bool trackball_controller::isPanning() const
{
    return panning_;
}

bool trackball_controller::is_rotating() const
{

    return rotating_;
}

void trackball_controller::beginRotate(float x, float y)
{
    // TODO end panning if already panning
    glm::vec2 location{ x, y };
    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    rotating_ = true;
}

void trackball_controller::beginPan()
{
    // TODO end rotating if already rotating
    panning_ = !rotating_;
}

void trackball_controller::update(float x, float y)
{
    glm::vec2 location{ x, y };

    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    // auto view_vector = glm::rotate(rotation_, VIEW * glm::length(position_));
    auto right_vector = glm::rotate(rotation_, RIGHT);
    auto up_vector = glm::rotate(rotation_, UP);
    if (panning_) {
        auto pan_rate = 10.0f;
        auto dp = currentLocation_ - lastLocation_;
        position_ -= pan_rate * right_vector * dp.x;
        position_ += pan_rate * up_vector * dp.y;
    } else if (rotating_) {
        auto d = currentLocation_ - lastLocation_;
        rotation_ = glm::angleAxis(-d.x, UP) * rotation_ * glm::angleAxis(-d.y, RIGHT);
        rotation_ = glm::normalize(rotation_);
    }
}

void trackball_controller::endRotate(float x, float y)
{
    glm::vec2 location{ x, y };

    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    rotating_ = false;
}

void trackball_controller::endPan()
{
    panning_ = false;
}

void trackball_controller::zoom(int direction)
{
    auto speed = 1.0f;
    if (!rotating_ && !panning_) {
        auto view_vector = glm::rotate(rotation_, VIEW);
        position_ += (speed * direction * view_vector);
    }
}
}
