#pragma onnce

#include <sigma/Window.hpp>
#include <sigma/OpenGL/GLContext.hpp>
#include <sigma/EventEmitter.hpp>
#include <sigma/EventListener.hpp>

struct SDL_Window;

class SDLWindow : public Window
{
public:
    static void initializeSDL(std::shared_ptr<Engine> inEngine);

    SDLWindow(std::shared_ptr<Engine> inEngine, const std::string &inTitle, std::size_t inWidth, std::size_t inHeight);

    virtual ~SDLWindow();

    virtual void initialize() override;

    virtual void swapBuffer() override;

    SDL_Window *handle();

private:
    std::string mTitle;
    std::size_t mWidth;
    std::size_t mHeight;
    std::shared_ptr<Engine> mEngine;

    SDL_Window* mWindow = nullptr;
};

class SDLEventEmitter : public EventEmitter
{
public:
    virtual bool process(const std::vector<std::weak_ptr<EventListener>> &inListeners) override;
};

class SDLGLContext : public GLContext
{
public:
    SDLGLContext(SDL_Window *inWindow);

    virtual ~SDLGLContext();

    virtual void makeCurrent(Window *inWindow) override;

private:
    void *mHandle = nullptr;
};
