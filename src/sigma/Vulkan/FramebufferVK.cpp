#include <sigma/Vulkan/FramebufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>

FramebufferVK::FramebufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFramebuffer, std::shared_ptr<RenderPassVK> inRenderPass, Rect<int32_t> inExtent)
    : mDevice(inDevice)
    , mFramebuffer(inFramebuffer)
	, mRenderPass(inRenderPass)
	, mExtent(inExtent)
{
}

FramebufferVK::~FramebufferVK()
{
    if (mDevice && mFramebuffer)
        vkDestroyFramebuffer(mDevice->handle(), mFramebuffer, nullptr);
}

std::shared_ptr<RenderPass> FramebufferVK::renderPass() const 
{
	return mRenderPass;
}

Rect<int32_t> FramebufferVK::extent() const
{
	return mExtent;
}
