#pragma once

#include <sigma/Event.hpp>
#include <sigma/EventListener.hpp>

#include <memory>

class Engine;
class Window;

class WindowEvent : public Event {
public:
    WindowEvent(std::weak_ptr<Window> inWindow, EventType inType);

    std::weak_ptr<Window> window() const;

private:
    std::weak_ptr<Window> mWindow;
};

class Window : public std::enable_shared_from_this<Window>, public EventListener {
public:
    virtual ~Window() = default;

    Window() = default;

    bool open() const { return mOpen; }

    virtual bool initialize() = 0;

    virtual void swapBuffer() = 0;

    virtual void processEvent(Event* inEvent) override;

protected:
    bool mOpen = false;
};
