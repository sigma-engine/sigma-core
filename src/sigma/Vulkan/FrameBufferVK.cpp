#include <sigma/Vulkan/FrameBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

FrameBufferVK::FrameBufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFrameBuffer, std::shared_ptr<RenderPassVK> inRenderPass, Rect<int32_t> inExtent)
    : mDevice(inDevice)
    , mFrameBuffer(inFrameBuffer)
    , mRenderPass(inRenderPass)
    , mExtent(inExtent)
{
}

FrameBufferVK::~FrameBufferVK()
{
    if (mDevice && mFrameBuffer)
        vkDestroyFramebuffer(mDevice->handle(), mFrameBuffer, nullptr);
}

std::shared_ptr<RenderPass> FrameBufferVK::renderPass() const
{
    return mRenderPass;
}

Rect<int32_t> FrameBufferVK::extent() const
{
    return mExtent;
}
