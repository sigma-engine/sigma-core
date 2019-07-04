#pragma once

class Event;

class EventListener {
public:
    virtual ~EventListener() = default;

    virtual void processEvent(Event* inEvent) = 0;
};