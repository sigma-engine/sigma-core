#include <sigma/Vulkan/FramebufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>

FramebufferVK::FramebufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFramebuffer)
    : mDevice(inDevice)
    , mFramebuffer(inFramebuffer)
{
}

FramebufferVK::~FramebufferVK()
{
    if (mDevice && mFramebuffer)
        vkDestroyFramebuffer(mDevice->handle(), mFramebuffer, nullptr);
}
