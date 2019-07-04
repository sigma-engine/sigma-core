#pragma once

#include <sigma/DeviceManager.hpp>

#include <vulkan/vulkan.h>

#include <vector>

class DeviceVK;

class DeviceManagerVK : public std::enable_shared_from_this<DeviceManagerVK>, public DeviceManager {
public:
    virtual ~DeviceManagerVK();

    virtual bool initialize(const std::set<std::string>& inRequiredExtensions) override;

    virtual void enumerateSurfaceDevices(std::shared_ptr<Surface> inSurface, std::vector<std::shared_ptr<Device>>& outDevices) override;

    VkInstance handle() { return mHandle; }

protected:
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT mDebugMessenger = nullptr;
#endif

    VkInstance mHandle = nullptr;
    std::vector<std::string> mRequiredExtensions;
    std::vector<std::string> mEnabledLayers;
    std::vector<std::shared_ptr<DeviceVK>> mDevices;
};
