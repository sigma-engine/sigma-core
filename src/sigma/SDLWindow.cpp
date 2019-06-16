#include <sigma/SDLWindow.hpp>

#include <sigma/Engine.hpp>
#include <sigma/GLContext.hpp>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <cassert>
#include <algorithm>


static std::shared_ptr<SDLEventEmitter> sEventEmitter = nullptr;

void SDLWindow::initializeSDL(std::shared_ptr<Engine> inEngine)
{
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            throw std::runtime_error(SDL_GetError());
        }
        sEventEmitter = std::make_shared<SDLEventEmitter>();
        inEngine->addEmitter(sEventEmitter);
    }
}

SDLWindow::SDLWindow(std::shared_ptr<Engine> inEngine, const std::string &inTitle, std::size_t inWidth, std::size_t inHeight)
    : mTitle(inTitle)
    , mWidth(inWidth)
    , mHeight(inHeight)
    , mEngine(inEngine)
{
}

SDLWindow::~SDLWindow()
{
    if (mWindow)
        SDL_DestroyWindow(mWindow);
}

void SDLWindow::initialize()
{
    //SDL_GL_SetAttribute(SDL_GL_RED_SIZE, context_attributes_.red);
    //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, context_attributes_.green);
    //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, context_attributes_.blue);
    //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, context_attributes_.alpha);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context_attributes_.depth);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context_attributes_.stencil);

    //if(context_attributes_.samples > 0) {
    //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, context_attributes_.samples);
    //}

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context_attributes_.major);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context_attributes_.minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    // TODO disable this in production.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //if(context_attributes_.core_profile)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mWidth), static_cast<int>(mHeight), SDL_WINDOW_OPENGL); //| SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
    if (!mWindow)
        throw std::runtime_error(SDL_GetError());

    SDL_SetWindowData(mWindow, "this", this);

    mContext = mEngine->graphicsContext();
    if (mContext == nullptr)
    {
        mContext = std::make_shared<SDLGLContext>(mWindow);
    }

    mContext->makeCurrent(this);


    SDL_GL_SetSwapInterval(0); //context_attributes_.vsync

    mOpen = true;
}

void SDLWindow::swapBuffer()
{
    SDL_GL_SwapWindow(mWindow);
}

SDL_Window *SDLWindow::handle()
{
    return mWindow;
}

bool SDLEventEmitter::process(const std::vector<std::weak_ptr<EventListener>> &inListeners)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            // TODO: handle all events here
            case SDL_WINDOWEVENT:
            {
                auto sdlWindow = SDL_GetWindowFromID(event.window.windowID);
                auto window = static_cast<SDLWindow*>(SDL_GetWindowData(sdlWindow, "this"));
                if (!window)
                    break;

                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_CLOSE:
                {
                    WindowEvent evt(window->shared_from_this(), EventType::WindowClose);
                    std::for_each(inListeners.begin(), inListeners.end(), [&](auto l) {
                        auto ptr = l.lock();
                        if (ptr)
                            ptr->processEvent(&evt);
                    });
                    break;
                }
                }
                break;
            }
        }
    }
    return true;
}

SDLGLContext::SDLGLContext(SDL_Window *inWindow)
{
    assert(inWindow);
    mHandle = SDL_GL_CreateContext(inWindow);
    gladLoadGL();
}

SDLGLContext::~SDLGLContext()
{
    if (mHandle)
        SDL_GL_DeleteContext(mHandle);
}

void SDLGLContext::makeCurrent(Window *inWindow)
{
    assert(dynamic_cast<SDLWindow*>(inWindow));
    auto sdlWindow = static_cast<SDLWindow*>(inWindow);
    SDL_GL_MakeCurrent(sdlWindow->handle(), mHandle);

}
