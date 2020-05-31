#include <SimpleCameraController.hpp>

#include <sigma/CameraComponent.hpp>
#include <sigma/Event.hpp>
#include <sigma/TransformComponent.hpp>


void SimpleCameraController::processEvent(Event* inEvent)
{
    switch (inEvent->type()) {
    case EventType::MouseWheel: {
        MouseWheelEvent* wheel = static_cast<MouseWheelEvent*>(inEvent);
        zoom(wheel->scrollY());
        break;
    }
    case EventType::MouseMove: {
        MouseEvent* move = static_cast<MouseEvent*>(inEvent);
        update(move->x() / 1920, move->y() / 1080);
        break;
    }
    case EventType::MouseButtonDown: {
        MouseButtonEvent* mouseButton = static_cast<MouseButtonEvent*>(inEvent);
        if (!mRotating)
            beginRotate(mouseButton->x() / 1920, mouseButton->y() / 1080);
        break;
    }
    case EventType::MouseButtonUp: {
        MouseButtonEvent* mouseButton = static_cast<MouseButtonEvent*>(inEvent);
        if (mRotating)
            endRotate(mouseButton->x() / 1920, mouseButton->y() / 1080);
        if (mPanning)
            endPan();
        break;
    }
    case EventType::KeyDown: {
        KeyboardEvent* keyboard = static_cast<KeyboardEvent*>(inEvent);
        if (!mPanning && keyboard->key() == KeyboardKey::LeftShift)
            beginPan();
        break;
    }
    case EventType::KeyUp: {
        KeyboardEvent* keyboard = static_cast<KeyboardEvent*>(inEvent);
        if (mPanning && keyboard->key() == KeyboardKey::LeftShift)
            endPan();
        break;
    }
    }
}

void SimpleCameraController::beginRotate(float x, float y)
{
    // TODO end panning if already panning
    glm::vec2 location{ x, y };
    mLastLocation = mCurrentLocation;
    mCurrentLocation = location;
    mRotating = true;
}

void SimpleCameraController::beginPan()
{
    // TODO end rotating if already rotating
    mPanning = !mRotating;
}

void SimpleCameraController::update(float x, float y)
{
    glm::vec2 location{ x, y };

    mLastLocation = mCurrentLocation;
    mCurrentLocation = location;
    // auto view_vector = glm::rotate(mRotation, VIEW * glm::length(mPosition));
    auto right_vector = glm::rotate(mTransform.rotation, RIGHT);
    auto up_vector = glm::rotate(mTransform.rotation, UP);
    if (mPanning) {
        auto pan_rate = 10.0f;
        auto dp = mCurrentLocation - mLastLocation;
        mTransform.position -= pan_rate * right_vector * dp.x;
        mTransform.position += pan_rate * up_vector * dp.y;
    } else if (mRotating) {
        auto d = mCurrentLocation - mLastLocation;
        mTransform.rotation = glm::angleAxis(d.x, UP) * mTransform.rotation * glm::angleAxis(-d.y, RIGHT);
        mTransform.rotation = glm::normalize(mTransform.rotation);
    }
}

void SimpleCameraController::endRotate(float x, float y)
{
    glm::vec2 location{ x, y };

    mLastLocation = mCurrentLocation;
    mCurrentLocation = location;
    mRotating = false;
}

void SimpleCameraController::endPan()
{
    mPanning = false;
}

void SimpleCameraController::zoom(float direction)
{
    auto speed = 0.1f;
    if (!mRotating && !mPanning) {
        auto view_vector = glm::rotate(mTransform.rotation, VIEW);
        mTransform.position += (speed * direction * view_vector);
    }
}