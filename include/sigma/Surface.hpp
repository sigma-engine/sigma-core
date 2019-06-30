#pragma once

#include <memory>

class DeviceManager;

class Surface {
public:
    virtual ~Surface() = default;

    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) = 0;
};
