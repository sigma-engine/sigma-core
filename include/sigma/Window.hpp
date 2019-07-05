#pragma once

#include <sigma/Event.hpp>
#include <sigma/EventListener.hpp>

#include <memory>
#include <set>
#include <string>

class Engine;
class Window;
class Surface;
enum class GraphicsAPI;

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

    virtual std::set<std::string> requiredExtensions(GraphicsAPI inGraphicsAPI) const = 0;

    virtual bool initialize() = 0;

    virtual std::shared_ptr<Surface> surface() = 0;

    virtual void processEvent(Event* inEvent) override;

protected:
    bool mOpen = false;
};
