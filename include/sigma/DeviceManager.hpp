#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

class Device;
class Surface;

class DeviceManager {
public:
    virtual ~DeviceManager() = default;

    virtual void sortDevices(std::vector<std::shared_ptr<Device>>& inDevices);

    virtual bool initialize(const std::set<std::string>& inRequiredExtensions) = 0;

    virtual void enumerateSurfaceDevices(std::shared_ptr<Surface> inSurface, std::vector<std::shared_ptr<Device>>& outDevices) = 0;
};
