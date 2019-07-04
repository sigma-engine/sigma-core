#pragma once

enum class EventType {
    WindowClose
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
