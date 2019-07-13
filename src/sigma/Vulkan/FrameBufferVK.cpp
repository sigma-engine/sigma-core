#include <sigma/Vulkan/FrameBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

FrameBufferVK::FrameBufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFrameBuffer, std::shared_ptr<RenderPassVK> inRenderPass, const glm::uvec2& inSize)
    : mDevice(inDevice)
    , mFrameBuffer(inFrameBuffer)
    , mRenderPass(inRenderPass)
    , mSize(inSize)
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

glm::uvec2 FrameBufferVK::size() const
{
    return mSize;
}
