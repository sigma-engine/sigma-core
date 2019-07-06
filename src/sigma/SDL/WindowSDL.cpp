#include <sigma/SDL/WindowSDL.hpp>

#include <sigma/Engine.hpp>
#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceManagerVK.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <algorithm>
#include <cassert>

static std::shared_ptr<EventEmitterSDL> sEventEmitter = nullptr;
static SDL_GLContext sGLContext = nullptr;

void WindowSDL::initializeSDL(std::shared_ptr<Engine> inEngine)
{
    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            throw std::runtime_error(SDL_GetError());
        }
        sEventEmitter = std::make_shared<EventEmitterSDL>();
        inEngine->addEmitter(sEventEmitter);
    }
}

WindowSDL::WindowSDL(std::shared_ptr<Engine> inEngine, const std::string& inTitle, uint32_t inWidth, uint32_t inHeight)
    : mTitle(inTitle)
    , mWidth(inWidth)
    , mHeight(inHeight)
    , mEngine(inEngine)
{

    int flags = SDL_WINDOW_SHOWN;
    switch (mEngine->graphicsAPI()) {
    case GraphicsAPI::OpenGL: {
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
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

        flags |= SDL_WINDOW_OPENGL;
        break;
    }
    case GraphicsAPI::Vulkan: {
        flags |= SDL_WINDOW_VULKAN;
        break;
    }
    default: {
        SIGMA_ASSERT(false, "Unknown Graphics API!");
        break;
    }
    }

    mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(mWidth), static_cast<int>(mHeight), static_cast<SDL_WindowFlags>(flags)); //| SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
    if (!mWindow)
        throw std::runtime_error(SDL_GetError());

    SDL_SetWindowData(mWindow, "this", this);

    switch (mEngine->graphicsAPI()) {
    case GraphicsAPI::OpenGL: {
        mSurface = std::make_shared<SurfaceSDLGL>(mWindow);
        break;
    }
    case GraphicsAPI::Vulkan: {
        mSurface = std::make_shared<SurfaceSDLVK>(mWindow);
        break;
    }
    default: {
        break;
    }
    }
}

WindowSDL::~WindowSDL()
{
    if (mWindow)
        SDL_DestroyWindow(mWindow);
}

std::set<std::string> WindowSDL::requiredExtensions(GraphicsAPI inGraphicsAPI) const
{
    std::set<std::string> extsSet;

    switch (inGraphicsAPI) {
    case GraphicsAPI::Vulkan: {
        uint32_t count;
        std::vector<const char*> exts;

        if (!SDL_Vulkan_GetInstanceExtensions(mWindow, &count, nullptr)) {
            SIGMA_ERROR(SDL_GetError());
        }

        exts.resize(count);

        if (!SDL_Vulkan_GetInstanceExtensions(mWindow, &count, exts.data())) {
            SIGMA_ERROR(SDL_GetError());
        }
        extsSet.insert(exts.begin(), exts.end());
        break;
    }
    default: {
        break;
    }
    }

    return extsSet;
}

bool WindowSDL::initialize()
{
    if (!mSurface->initialize(mEngine->deviceManager(), mWidth, mHeight)) {
        return false;
    }
    mOpen = true;
    return true;
}

std::shared_ptr<Surface> WindowSDL::surface()
{
    return mSurface;
}

SDL_Window* WindowSDL::handle()
{
    return mWindow;
}

bool EventEmitterSDL::process(const std::vector<std::weak_ptr<EventListener>>& inListeners)
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        // TODO: handle all events here
        case SDL_WINDOWEVENT: {
            auto handle = SDL_GetWindowFromID(event.window.windowID);
            auto window = static_cast<WindowSDL*>(SDL_GetWindowData(handle, "this"));
            if (!window)
                break;

            switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE: {
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

SurfaceSDLGL::SurfaceSDLGL(SDL_Window* inWindow)
{
    mWindow = inWindow;
    if (sGLContext == nullptr)
        sGLContext = SDL_GL_CreateContext(mWindow);

    mHandle = sGLContext;
    SDL_GL_MakeCurrent(mWindow, mHandle);
}

SurfaceSDLGL::~SurfaceSDLGL()
{
    /* if (mHandle)
        SDL_GL_DeleteContext(mHandle);
	*/
}

bool SurfaceSDLGL::initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight)
{
    // TODO: Pixel format
    SDL_GL_MakeCurrent(mWindow, mHandle);
    return SurfaceGL::initialize(inDevice, inWidth, inHeight);
}

void SurfaceSDLGL::beginFrame(SurfaceFrameData& outData)
{
    SDL_GL_MakeCurrent(mWindow, mHandle);
    SurfaceGL::beginFrame(outData);
}

void SurfaceSDLGL::endFrame(const SurfaceFrameData& inData)
{
    SDL_GL_MakeCurrent(mWindow, mHandle);
    SDL_GL_SwapWindow(mWindow);
}

SurfaceSDLVK::SurfaceSDLVK(SDL_Window* inWindow)
    : mWindow(inWindow)
{
}

bool SurfaceSDLVK::initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<DeviceManagerVK>(inDevice) != nullptr, "Incorrect Device type");
    mInstance = std::static_pointer_cast<DeviceManagerVK>(inDevice);
    mWidth = inWidth;
    mHeight = inHeight;
    if (!SDL_Vulkan_CreateSurface(mWindow, mInstance->handle(), &mSurface)) {
        SIGMA_ERROR(SDL_GetError());
        return false;
    }

    return true;
}
