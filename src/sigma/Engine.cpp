#include <sigma/Engine.hpp>

#include <sigma/Device.hpp>
#include <sigma/EventEmitter.hpp>
#include <sigma/Log.hpp>
#include <sigma/OpenGL/DeviceManagerGL.hpp>
#include <sigma/SDL/WindowSDL.hpp>
#include <sigma/Resource/AssetManager.hpp>

#ifdef SIGMA_VULKAN_SUPPORT
#include <sigma/Vulkan/DeviceManagerVK.hpp>
#endif

#include <spdlog/sinks/stdout_color_sinks.h>

#include <algorithm>

std::shared_ptr<Engine> Engine::create()
{
	return std::make_shared<Engine>();
}

GraphicsAPI Engine::graphicsAPI() const
{
	return mGraphicsAPI;
}

bool Engine::initialize(GraphicsAPI inGraphicsAPI)
{
	mGraphicsAPI = inGraphicsAPI;
	mConsole = spdlog::stdout_color_mt(SIGMA_LOG_NAME);

	switch (inGraphicsAPI)
	{
	case GraphicsAPI::OpenGL:
	{
		mDeviceManger = std::make_shared<DeviceManagerGL>();
		break;
	}
#ifdef SIGMA_VULKAN_SUPPORT
	case GraphicsAPI::Vulkan:
	{
		mDeviceManger = std::make_shared<DeviceManagerVK>();
		break;
	}
#endif
	default:
	{
		return false;
	}
	}

	return true;
}

std::shared_ptr<Window> Engine::createWindow(const std::string &inTitle, uint32_t inWidth, uint32_t inHeight)
{
	std::shared_ptr<Window> window = nullptr;
	switch (mGraphicsAPI)
	{
#ifdef SIGMA_VULKAN_SUPPORT
	case GraphicsAPI::Vulkan:
#endif
	case GraphicsAPI::OpenGL:
	{
		WindowSDL::initializeSDL(shared_from_this());
		window = std::make_shared<WindowSDL>(shared_from_this(), inTitle, inWidth, inHeight);
		break;
	}
	default:
	{
		SIGMA_ASSERT(false, "Unknown Graphics API");
		break;
	}
	}

	if (!mDeviceInitialized)
	{
		auto exts = window->requiredExtensions(mGraphicsAPI);
		mRequiredExtensions[mGraphicsAPI].insert(exts.begin(), exts.end());

		if (mDeviceManger->initialize(mRequiredExtensions[mGraphicsAPI]))
			mDeviceInitialized = true;
		else
			window = nullptr;
	}

	if (window && window->initialize())
		mEventListeners.push_back(window);
	else
		window = nullptr;

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

std::shared_ptr<DeviceManager> Engine::deviceManager()
{
	return mDeviceManger;
}

bool Engine::process()
{
	auto listIt = std::remove_if(mEventListeners.begin(), mEventListeners.end(), [](auto a) { return a.lock() == nullptr; });
	mEventListeners.erase(listIt, mEventListeners.end());

	auto emitIt = std::remove_if(mEventEmitters.begin(), mEventEmitters.end(), [&](auto e) {
		auto ptr = e.lock();
		if (ptr)
			return !ptr->process(mEventListeners);
		else
			return true;
	});
	mEventEmitters.erase(emitIt, mEventEmitters.end());

	return true;
}
