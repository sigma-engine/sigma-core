#include <SimpleGame.hpp>

#include <SimpleCameraController.hpp>
#include <SimpleRenderer.hpp>
#include <sigma/Algorithm.hpp>
#include <sigma/CameraComponent.hpp>
#include <sigma/Device.hpp>
#include <sigma/DeviceManager.hpp>
#include <sigma/Engine.hpp>
#include <sigma/Event.hpp>
#include <sigma/Log.hpp>
#include <sigma/TransformComponent.hpp>
#include <sigma/Window.hpp>
#include <sigma/Resource/GLTF2Loader.hpp>

#include <SimpleGame.hpp>

#include <entt/entt.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

int SimpleGame::run(const std::vector<std::string> &inArguments)
{
    bool useOpenGL = std::find_if(inArguments.begin(), inArguments.end(), [](auto a) { return a == "--opengl"; }) != inArguments.end();

    auto engine = Engine::create();
    if (!engine->initialize(useOpenGL ? GraphicsAPI::OpenGL : GraphicsAPI::Vulkan))
        return -1;

    auto deviceManager = engine->deviceManager();
    if (deviceManager == nullptr)
        return -1;

    auto window = engine->createWindow(fmt::format("Simple Game - ({})", engine->graphicsAPI() == GraphicsAPI::OpenGL ? "OpenGL" : "Vulkan"), 1920 - 50, 1080 - 50);
    if (window == nullptr)
        return -1;

    std::vector<std::shared_ptr<Device>> surfaceDevices;
    deviceManager->enumerateSurfaceDevices(window->surface(), surfaceDevices);
    if (surfaceDevices.empty())
        return -1;

    auto device = surfaceDevices[0];
    if (!device->initialize({window->surface()}))
        return -1;

    auto renderer = std::make_shared<SimpleRenderer>(engine, device, window->surface());
    if (!renderer->initialize())
        return -1;

    mCameraController = std::make_shared<SimpleCameraController>(mRegistry);
    engine->addListener(mCameraController);

    /* if (!inArguments.empty() && !starts_with(inArguments.back(), "-"))
        loadRegistry(inArguments.back(), mRegistry);*/

    GLTF2Loader loader;
    // if (!loader.load("C:/Users/aaron/projects/personal/sigma-engine/models/2.0/Box/glTF/Box.gltf"))
    // if (!loader.load("C:/Users/aaron/projects/personal/sigma-engine/models/2.0/AntiqueCamera/glTF/AntiqueCamera.gltf"))
    // if (!loader.load("C:/Users/aaron/projects/personal/sigma-engine/models/2.0/Sponza/glTF/Sponza.gltf"))
    if (!loader.load("C:/Users/aaron/projects/personal/sigma-engine/models/2.0/FlightHelmet/glTF/FlightHelmet.gltf"))
    {
        return -1;
    }

    auto cameras = mRegistry.view<CameraComponent, TransformComponent>();
    if (cameras.size() == 0)
    {
        auto &[entity, camera, transfrom] = mRegistry.create<CameraComponent, TransformComponent>();
        transfrom.position = glm::vec3(0, 0, 1);
    }

    while (window->open() && engine->process())
    {
        renderer->render(mCameraController->attachedEntity(), mRegistry);
    }

    return 0;
}