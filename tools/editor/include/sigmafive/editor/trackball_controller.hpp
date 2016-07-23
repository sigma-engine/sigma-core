#ifndef CPPBR_MATH_TRACKBALL_CONTROLLER_HPP
#define CPPBR_MATH_TRACKBALL_CONTROLLER_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

static const glm::vec3 RIGHT = { 1, 0, 0 };
static const glm::vec3 UP = { 0, 1, 0 };
static const glm::vec3 VIEW = { 0, 0, -1 };

class trackball_controller {
public:
    trackball_controller()
        : rotating_(false)
        , panning_(false)
    {
        position_ = glm::vec3{ 10, 0, 1 };
        rotation_ = glm::quat(glm::vec3{ glm::radians(90.0f), 0, glm::radians(90.0f) });
    }

    glm::mat4 matrix() const
    {
        glm::mat4 mat;
        return glm::inverse(glm::translate(mat, position_) * glm::mat4_cast(rotation_));
    }

    void begin_rotate(glm::vec2 location)
    {
        lastLocation_ = currentLocation_;
        currentLocation_ = location;
        rotating_ = true;
    }

    void begin_pan() { panning_ = !rotating_; }

    void update(glm::vec2 location)
    {
        lastLocation_ = currentLocation_;
        currentLocation_ = location;
        auto view_vector = glm::rotate(rotation_, VIEW * glm::length(position_));
        auto right_vector = glm::rotate(rotation_, RIGHT);
        auto up_vector = glm::rotate(rotation_, UP);
        if (panning_) {
            auto pan_rate = -10.0f;
            auto dp = currentLocation_ - lastLocation_;
            position_ += pan_rate * right_vector * dp.x;
            position_ += pan_rate * up_vector * dp.y;
        } else if (rotating_) {
            auto d = currentLocation_ - lastLocation_;
            rotation_ = glm::angleAxis(-d.x, glm::vec3{ 0, 0, 1 }) * rotation_ * glm::angleAxis(d.y, RIGHT);
            rotation_ = glm::normalize(rotation_);
        }
    }

    void end_rotate(glm::vec2 location)
    {
        lastLocation_ = currentLocation_;
        currentLocation_ = location;
        rotating_ = false;
    }

    void end_pan() { panning_ = false; }

    void zoom(int direction)
    {
        auto speed = .008f;
        if (!rotating_ && !panning_) {
            auto view_vector = glm::rotate(rotation_, glm::vec3{ 0, 0, -1 });
            position_ += (speed * direction * view_vector);
        }
    }

    glm::vec2 currentLocation_, lastLocation_;
    glm::vec3 position_;
    glm::quat rotation_;
    bool rotating_;
    bool panning_;
};

#endif // CPPBR_MATH_TRACKBALL_CONTROLLER_HPP
