#pragma once

#include <sigma/DeviceManager.hpp>

class DeviceManagerGL : public DeviceManager {
public:
	DeviceManagerGL();

	virtual bool initialize(const std::set<std::string>& inRequiredExtensions) override;

	virtual void enumerateSurfaceDevices(std::shared_ptr<Surface> inSurface, std::vector<std::shared_ptr<Device>>& outDevices) override;

private:
	std::set<std::string> mExtensions;
	std::shared_ptr<Device> mGraphicsDevice = nullptr;
};
