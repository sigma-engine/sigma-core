#include <SimpleGame.hpp>

#include <sigma/Context.hpp>
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
#include <sigma/Resource/StbImageLoader.hpp>
#include <sigma/Resource/AssetManager.hpp>

#include <SimpleGame.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

int SimpleGame::run(const std::vector<std::string> &inArguments)
{
	bool useOpenGL = std::find_if(inArguments.begin(), inArguments.end(), [](auto a) { return a == "--opengl"; }) != inArguments.end();

	auto engine = Engine::create();
	GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;

#ifdef SIGMA_VULKAN_SUPPORT
	if (!useOpenGL) graphicsAPI = GraphicsAPI::Vulkan;
#endif

	if (!engine->initialize(graphicsAPI))
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

	auto context = std::make_shared<Context>();
	if (!context->initialize(engine, device))
		return -1;

	context->assetManager()->addLoader(std::make_shared<StbImageLoader>());

	auto renderer = std::make_shared<SimpleRenderer>(context, window->surface());
	if (!renderer->initialize())
		return -1;

	mCameraController = std::make_shared<SimpleCameraController>();
	engine->addListener(mCameraController);

	while (window->open() && engine->process())
	{
		renderer->render(mCameraController->transform());
	}

	return 0;
}