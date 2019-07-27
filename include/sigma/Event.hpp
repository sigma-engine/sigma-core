#pragma once

enum class EventType {
    WindowClose,
    MouseMove,
    MouseButtonDown,
    MouseButtonUp,
    MouseWheel,
    KeyDown,
    KeyUp
};

enum class MouseButton {
    Left,
    Right,
    Middle
};

enum class KeyboardKey {
    LeftShift
};

class Event {
public:
    Event(EventType inType)
        : mType(inType)
    {
    }

    EventType type() const { return mType; }

private:
    EventType mType;
};

class MouseEvent : public Event {
public:
    MouseEvent(EventType inType, float inX, float inY)
        : Event(inType)
        , mX(inX)
        , mY(inY)
    {
    }

    float x() const { return mX; }

    float y() const { return mY; }

private:
    float mX, mY;
};

class MouseWheelEvent : public Event {
public:
    MouseWheelEvent(float inScrollX, float inScrollY)
        : Event(EventType::MouseWheel)
        , mScrollX(inScrollX)
        , mScrollY(inScrollY)
    {
    }

    float scrollX() const { return mScrollX; }

    float scrollY() const { return mScrollY; }

private:
    float mScrollX, mScrollY;
};

class MouseButtonEvent : public MouseEvent {
public:
    MouseButtonEvent(float inX, float inY, MouseButton inButton, bool inDown)
        : MouseEvent(inDown ? EventType::MouseButtonDown : EventType::MouseButtonUp, inX, inY)
        , mButton(inButton)
    {
    }

    MouseButton button() const { return mButton; }

private:
    MouseButton mButton;
};

class KeyboardEvent : public Event {
public:
    KeyboardEvent(KeyboardKey inKey, bool inDown)
        : Event(inDown ? EventType::KeyDown : EventType::KeyUp)
        , mKey(inKey)
    {
    }

    KeyboardKey key() const { return mKey; }

private:
    KeyboardKey mKey;
};