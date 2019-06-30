#include <sigma/DeviceManager.hpp>

#include <sigma/Device.hpp>

#include <algorithm>


void DeviceManager::sortDevices(std::vector<std::shared_ptr<Device>> &inDevices)
{
    std::sort(inDevices.begin(), inDevices.end(), [](auto a, auto b) {
        return a->type() < b->type();
    });
}
