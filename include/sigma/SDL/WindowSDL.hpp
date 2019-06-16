#pragma onnce

#include <sigma/EventEmitter.hpp>
#include <sigma/EventListener.hpp>
#include <sigma/Window.hpp>
#include <sigma/OpenGL/SurfaceGL.hpp>
#include <sigma/Vulkan/SurfaceVK.hpp>

struct SDL_Window;
class Device;

class SurfaceSDLGL : public SurfaceGL {
public:
    SurfaceSDLGL(SDL_Window* inWindow);

    virtual ~SurfaceSDLGL();
private:
    SDL_Window *mWindow = nullptr;
    void* mHandle = nullptr;
};

class SurfaceSDLVK : public SurfaceVK {
public:
    SurfaceSDLVK(SDL_Window *inWindow);

    bool initialize(std::shared_ptr<Device> inDevice) override;
private:
    SDL_Window *mWindow = nullptr;
};

class EventEmitterSDL : public EventEmitter {
public:
    virtual bool process(const std::vector<std::weak_ptr<EventListener>>& inListeners) override;
};

class WindowSDL : public Window {
public:
    static void initializeSDL(std::shared_ptr<Engine> inEngine);

    WindowSDL(std::shared_ptr<Engine> inEngine, const std::string& inTitle, std::size_t inWidth, std::size_t inHeight);

    virtual ~WindowSDL();

    virtual std::set<std::string> requiredExtensions(GraphicsAPI inGraphicsAPI) const override;

    virtual bool initialize() override;

    virtual void swapBuffer() override;

    SDL_Window* handle();

private:
    std::string mTitle;
    std::size_t mWidth;
    std::size_t mHeight;
    std::shared_ptr<Engine> mEngine = nullptr;
    std::shared_ptr<Surface> mSurface = nullptr;

    SDL_Window* mWindow = nullptr;
};
