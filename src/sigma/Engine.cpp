#include <sigma/Engine.hpp>

#include <sigma/SDLWindow.hpp>
#include <sigma/EventEmitter.hpp>
#include <sigma/Log.hpp>

#include <algorithm>

std::shared_ptr<Engine> Engine::create()
{
    return std::make_shared<Engine>();
}

GraphicsAPI Engine::graphicsAPI() const
{
    return mGraphicsAPI;
}

void Engine::initialize(GraphicsAPI inGraphicsAPI)
{
    mGraphicsAPI = inGraphicsAPI;
    mConsole = spdlog::stdout_color_mt(SIGMA_LOG_NAME);
}

std::shared_ptr<Window> Engine::createWindow(const std::string &inTitle, std::size_t inWidth, std::size_t inHeight)
{
    std::shared_ptr<Window> window = nullptr;
    switch (mGraphicsAPI) {
    case GraphicsAPI::OpenGL:
    {
        SDLWindow::initializeSDL(shared_from_this());
        window = std::make_shared<SDLWindow>(shared_from_this(), inTitle, inWidth, inHeight);
        window->initialize();
        if (mContext == nullptr)
        {
            mContext = window->graphicsContext();
            mContext->initialize();
        }

        break;
    }
    case GraphicsAPI::None:
    {
        window = nullptr;
        break;
    }
    }

    if (window)
        mEventListeners.push_back(window);

    return window;
}


void Engine::addEmitter(std::weak_ptr<EventEmitter> inEmitter)
{
    mEventEmitters.push_back(inEmitter);
}

void Engine::removeEmitter(std::weak_ptr<EventEmitter> inEmitter)
{
    auto it = std::remove_if(mEventEmitters.begin(), mEventEmitters.end(), [&](auto e) { return e.lock() == inEmitter.lock(); });
    mEventEmitters.erase(it, mEventEmitters.end());
}

void Engine::addListener(std::weak_ptr<EventListener> inListener)
{
    mEventListeners.push_back(inListener);
}

void Engine::removeListener(std::weak_ptr<EventListener> inListener)
{
    auto it = std::remove_if(mEventListeners.begin(), mEventListeners.end(), [&](auto e) { return e.lock() == inListener.lock(); });
    mEventListeners.erase(it, mEventListeners.end());
}

std::shared_ptr<Context> Engine::graphicsContext()
{
    return mContext;
}

bool Engine::process()
{
    auto listIt = std::remove_if(mEventListeners.begin(), mEventListeners.end(), [](auto a) { return a.lock() == nullptr; });
    mEventListeners.erase(listIt, mEventListeners.end());

    auto emitIt = std::remove_if(mEventEmitters.begin(), mEventEmitters.end(), [&](auto e){
        auto ptr = e.lock();
        if (ptr)
            return !ptr->process(mEventListeners);
        else
            return true;
    });
    mEventEmitters.erase(emitIt, mEventEmitters.end());

    return true;
}
