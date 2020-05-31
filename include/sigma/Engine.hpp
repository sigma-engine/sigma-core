#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace spdlog
{
class logger;
}

enum class GraphicsAPI
{
	Unknown,
	OpenGL,
#ifdef SIGMA_VULKAN_SUPPORT
	Vulkan
#endif
};

class Window;
class DeviceManager;
class EventListener;
class EventEmitter;

class Engine : public std::enable_shared_from_this<Engine>
{
public:
	virtual ~Engine() = default;

	GraphicsAPI graphicsAPI() const;

	virtual bool initialize(GraphicsAPI inGraphicsAPI);

	std::shared_ptr<Window> createWindow(const std::string &inTitle, uint32_t inWidth, uint32_t inHeight);

	void addEmitter(std::weak_ptr<EventEmitter> inEmitter);

	void removeEmitter(std::weak_ptr<EventEmitter> inEmitter);

	void addListener(std::weak_ptr<EventListener> inListener);

	void removeListener(std::weak_ptr<EventListener> inListener);

	std::shared_ptr<DeviceManager> deviceManager();

	bool process();

	static std::shared_ptr<Engine> create();

private:
	GraphicsAPI mGraphicsAPI;
	bool mDeviceInitialized = false;
	std::map<GraphicsAPI, std::set<std::string>> mRequiredExtensions;
	std::shared_ptr<DeviceManager> mDeviceManger = nullptr;
	std::shared_ptr<spdlog::logger> mConsole = nullptr;
	std::vector<std::weak_ptr<EventEmitter>> mEventEmitters;
	std::vector<std::weak_ptr<EventListener>> mEventListeners;
};
