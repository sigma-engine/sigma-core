#pragma once

#include <sigma/Surface.hpp>

#include <vulkan/vulkan.h>

class DeviceVK;

class SurfaceVK : public Surface {
public:
    virtual ~SurfaceVK();

protected:
    std::shared_ptr<DeviceVK> mInstance = nullptr;
    VkSurfaceKHR mSurface = nullptr;
};
