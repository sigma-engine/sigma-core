#ifndef SIGMA_TRACKBALL_CONTROLLER_HPP
#define SIGMA_TRACKBALL_CONTROLLER_HPP

#include <QQuickItem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sigma {
static const glm::vec3 RIGHT = { 1, 0, 0 };
static const glm::vec3 UP = { 0, 1, 0 };
static const glm::vec3 VIEW = { 0, 0, -1 };

class TrackballController : public QQuickItem {
    Q_OBJECT
public:
    Q_INVOKABLE TrackballController(QQuickItem* parent = nullptr);

    glm::mat4 matrix() const;

    Q_INVOKABLE bool isPanning() const;

    Q_INVOKABLE void beginRotate(float x, float y);

    Q_INVOKABLE void beginPan(float x, float y);

    Q_INVOKABLE void update(float x, float y);

    Q_INVOKABLE void endRotate(float x, float y);

    Q_INVOKABLE void endPan(float x, float y);

    Q_INVOKABLE void zoom(int direction);

signals:
    void viewChanged();

private:
    glm::vec2 currentLocation_, lastLocation_;
    glm::vec3 position_;
    glm::quat rotation_;
    bool rotating_;
    bool panning_;
};
}

#endif // SIGMA_TRACKBALL_CONTROLLER_HPP
