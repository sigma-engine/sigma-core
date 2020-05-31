#pragma once

#include <sigma/EventListener.hpp>
#include <sigma/TransformComponent.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

class Event;
struct CameraComponent;

class SimpleCameraController : public EventListener {
public:
	const TransformComponent& transform() const { return mTransform; }

	virtual void processEvent(Event* inEvent) override;

private:
	const glm::vec3 RIGHT = { 1, 0, 0 };
	const glm::vec3 UP = { 0, 1, 0 };
	const glm::vec3 VIEW = { 0, 0, -1 };

	glm::vec2 mCurrentLocation = { 0, 0 };
	glm::vec2 mLastLocation = { 0, 0 };
	bool mRotating = false;
	bool mPanning = false;

	TransformComponent mTransform;

	void beginRotate(float x, float y);

	void beginPan();

	void update(float x, float y);

	void endRotate(float x, float y);

	void endPan();

	void zoom(float direction);
};