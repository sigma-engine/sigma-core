#include <sigma/Window.hpp>
#include <sigma/Event.hpp>

WindowEvent::WindowEvent(std::weak_ptr<Window> inWindow, EventType inType)
    : Event (inType)
    , mWindow(inWindow)
{
}

std::weak_ptr<Window> WindowEvent::window() const
{
    return mWindow;
}

std::shared_ptr<Context> Window::graphicsContext() { return mContext; }

void Window::processEvent(Event *inEvent)
{
    if (inEvent->type() == EventType::WindowClose)
    {
        auto window = static_cast<WindowEvent*>(inEvent)->window().lock();
        if (window.get() == this)
            window->mOpen = false;
    }
}
