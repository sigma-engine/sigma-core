#pragma once

#include <sigma/Framebuffer.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;

class FramebufferVK : public Framebuffer {
public:
    FramebufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFramebuffer);

    virtual ~FramebufferVK();

    VkFramebuffer handle() const { return mFramebuffer; }

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkFramebuffer mFramebuffer = nullptr;
};
