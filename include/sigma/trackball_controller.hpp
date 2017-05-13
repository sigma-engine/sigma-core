#ifndef SIGMA_TRACKBALL_CONTROLLER_HPP
#define SIGMA_TRACKBALL_CONTROLLER_HPP

#include <sigma/config.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sigma {
static const glm::vec3 RIGHT = { 1, 0, 0 };
static const glm::vec3 UP = { 0, 1, 0 };
static const glm::vec3 VIEW = { 0, 0, -1 };

class SIGMA_API trackball_controller {
public:
    trackball_controller();

    glm::mat4 matrix() const;

    bool isPanning() const;

    bool is_rotating() const;

    void beginRotate(float x, float y);

    void beginPan();

    void update(float x, float y);

    void endRotate(float x, float y);

    void endPan();

    void zoom(int direction);

private:
    glm::vec2 currentLocation_, lastLocation_;
    glm::vec3 position_;
    glm::quat rotation_;
    bool rotating_;
    bool panning_;
};
}

#endif // SIGMA_TRACKBALL_CONTROLLER_HPP
