#include <SimpleCameraController.hpp>

#include <sigma/CameraComponent.hpp>
#include <sigma/Event.hpp>
#include <sigma/TransformComponent.hpp>


SimpleCameraController::SimpleCameraController(entt::registry& inRegistry)
    : mRegistry(inRegistry)
    , mCameraEntity(entt::null)
{
    mRegistry.on_destroy<CameraComponent>().connect<&SimpleCameraController::onCameraRemoved>(*this);
    mRegistry.on_destroy<CameraComponent>().connect<&SimpleCameraController::onCameraRemoved>(*this);

    mRegistry.on_construct<CameraComponent>().connect<&SimpleCameraController::onCameraCreated>(*this);
    mRegistry.on_construct<TransformComponent>().connect<&SimpleCameraController::onTransformCreated>(*this);
}

SimpleCameraController::~SimpleCameraController()
{
    mRegistry.on_destroy<CameraComponent>().disconnect<&SimpleCameraController::onCameraRemoved>(*this);
    mRegistry.on_destroy<CameraComponent>().disconnect<&SimpleCameraController::onCameraRemoved>(*this);

    mRegistry.on_construct<CameraComponent>().disconnect<&SimpleCameraController::onCameraCreated>(*this);
    mRegistry.on_construct<TransformComponent>().disconnect<&SimpleCameraController::onTransformCreated>(*this);
}

void SimpleCameraController::onCameraCreated(entt::entity inEntity, entt::registry& inRegistry, CameraComponent& inComponent)
{
    if (mCameraEntity == entt::null)
        tryAttachCamera(inEntity);
}

void SimpleCameraController::onTransformCreated(entt::entity inEntity, entt::registry& inRegistry, TransformComponent& inComponent)
{
    if (mCameraEntity == entt::null)
        tryAttachCamera(inEntity);
}

void SimpleCameraController::onCameraRemoved(entt::entity inEntity, entt::registry& inRegistry)
{
    if (inEntity == mCameraEntity)
        mCameraEntity = entt::null;
}

void SimpleCameraController::tryAttachCamera(entt::entity inEntity)
{
    auto [camera, transform] = mRegistry.try_get<CameraComponent, TransformComponent>(inEntity);
    if (camera != nullptr && transform != nullptr) {
        mPosition = transform->position;
        mRotation = transform->rotation;
        mCameraEntity = inEntity;
    }
}

void SimpleCameraController::processEvent(Event* inEvent)
{
    if (mCameraEntity != entt::null) {
        auto& [camera, transform] = mRegistry.get<CameraComponent, TransformComponent>(mCameraEntity);
        mPosition = transform.position;
        mRotation = transform.rotation;
    }

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

    if (mCameraEntity != entt::null) {
        auto& [camera, transform] = mRegistry.get<CameraComponent, TransformComponent>(mCameraEntity);
        transform.position = mPosition;
        transform.rotation = mRotation;
    }
}

entt::entity SimpleCameraController::attachedEntity() const
{
    return mCameraEntity;
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
    auto right_vector = glm::rotate(mRotation, RIGHT);
    auto up_vector = glm::rotate(mRotation, UP);
    if (mPanning) {
        auto pan_rate = 10.0f;
        auto dp = mCurrentLocation - mLastLocation;
        mPosition -= pan_rate * right_vector * dp.x;
        mPosition += pan_rate * up_vector * dp.y;
    } else if (mRotating) {
        auto d = mCurrentLocation - mLastLocation;
        mRotation = glm::angleAxis(d.x, UP) * mRotation * glm::angleAxis(-d.y, RIGHT);
        mRotation = glm::normalize(mRotation);
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
        auto view_vector = glm::rotate(mRotation, VIEW);
        mPosition += (speed * direction * view_vector);
    }
}