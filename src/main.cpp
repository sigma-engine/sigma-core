#include <sigma/Device.hpp>
#include <sigma/DeviceManager.hpp>
#include <sigma/Engine.hpp>
#include <sigma/Window.hpp>
#include <SimpleRenderer.hpp>
#include <sigma/Log.hpp>

int main(int argc, char const* argv[])
{
    (void)argc;
    (void)argv;
    auto engine = Engine::create();
    if (!engine->initialize(argc > 1 ? GraphicsAPI::OpenGL : GraphicsAPI::Vulkan))
        return -1;

    auto deviceManager = engine->deviceManager();
    if (deviceManager == nullptr)
        return -1;

    auto window = engine->createWindow(fmt::format("Simple Game - ({})", engine->graphicsAPI() == GraphicsAPI::OpenGL ? "OpenGL" : "Vulkan"), 1920 / 2, 1080 / 2);
    if (window == nullptr)
        return -1;

    std::vector<std::shared_ptr<Device>> surfaceDevices;
    deviceManager->enumerateSurfaceDevices(window->surface(), surfaceDevices);
    if (surfaceDevices.empty())
        return -1;

    auto device = surfaceDevices[0];
    if (!device->initialize({ window->surface() }))
        return -1;

	auto renderer = std::make_shared<SimpleRenderer>(engine, device, window->surface());
	if (!renderer->initialize())
		return -1;

    while (window->open() && engine->process()) {
		renderer->render();
    }
}
