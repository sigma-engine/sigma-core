#include <TrackballController.hpp>

namespace sigma {
TrackballController::TrackballController(QQuickItem* parent)
    : QQuickItem(parent)
    , rotating_(false)
    , panning_(false)
{
    //position_ = glm::vec3{ 10, 0, 1 };
    //rotation_ = glm::quat(glm::vec3{ glm::radians(90.0f), 0, glm::radians(90.0f) });
}

glm::mat4 TrackballController::matrix() const
{
    glm::mat4 mat;
    return glm::inverse(glm::translate(mat, position_) * glm::mat4_cast(rotation_));
}

bool TrackballController::isPanning() const
{
    return panning_;
}

void TrackballController::beginRotate(float x, float y)
{
    // TODO end panning if already panning
    glm::vec2 location{ x, y };
    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    rotating_ = true;
    emit viewChanged();
}

void TrackballController::beginPan(float x, float y)
{
    // TODO end rotating if already rotating
    panning_ = !rotating_;
    emit viewChanged();
}

void TrackballController::update(float x, float y)
{
    glm::vec2 location{ x, y };

    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    auto view_vector = glm::rotate(rotation_, VIEW * glm::length(position_));
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
    emit viewChanged();
}

void TrackballController::endRotate(float x, float y)
{
    glm::vec2 location{ x, y };

    lastLocation_ = currentLocation_;
    currentLocation_ = location;
    rotating_ = false;
    emit viewChanged();
}

void TrackballController::endPan(float x, float y)
{
    panning_ = false;
    emit viewChanged();
}

void TrackballController::zoom(int direction)
{
    auto speed = 1.0f;
    if (!rotating_ && !panning_) {
        auto view_vector = glm::rotate(rotation_, VIEW);
        position_ += (speed * direction * view_vector);
    }
    emit viewChanged();
}
}
