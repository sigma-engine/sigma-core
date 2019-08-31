#pragma once

#include <sigma/EventListener.hpp>

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

class Event;
struct CameraComponent;
struct TransformComponent;

class SimpleCameraController : public EventListener {
public:
    SimpleCameraController(entt::registry& inRegistry);

    ~SimpleCameraController();

    void onCameraCreated(entt::entity inEntity, entt::registry& inRegistry, CameraComponent& inComponent);

    void onTransformCreated(entt::entity inEntity, entt::registry& inRegistry, TransformComponent& inComponent);

    void onCameraRemoved(entt::entity inEntity, entt::registry& inRegistry);

    void tryAttachCamera(entt::entity inEntity);

    virtual void processEvent(Event* inEvent) override;

    entt::entity attachedEntity() const;

private:
    const glm::vec3 RIGHT = { 1, 0, 0 };
    const glm::vec3 UP = { 0, 1, 0 };
    const glm::vec3 VIEW = { 0, 0, -1 };

    entt::registry& mRegistry;
    entt::entity mCameraEntity;

    glm::vec2 mCurrentLocation = { 0, 0 };
    glm::vec2 mLastLocation = { 0, 0 };
    glm::vec3 mPosition = { 0, 0, 0 };
    glm::quat mRotation = { 0, 0, 0, 1 };
    bool mRotating = false;
    bool mPanning = false;

    void beginRotate(float x, float y);

    void beginPan();

    void update(float x, float y);

    void endRotate(float x, float y);

    void endPan();

    void zoom(float direction);
};