#pragma once

#include <sigma/Framebuffer.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class RenderPassVK;

class FramebufferVK : public Framebuffer {
public:
    FramebufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFramebuffer, std::shared_ptr<RenderPassVK> inRenderPass, Rect<int32_t> inExtent);

    virtual ~FramebufferVK();

	virtual std::shared_ptr<RenderPass> renderPass() const override;

	virtual Rect<int32_t> extent() const override;

    VkFramebuffer handle() const { return mFramebuffer; }

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
	std::shared_ptr<RenderPassVK> mRenderPass = nullptr;
    VkFramebuffer mFramebuffer = nullptr;
	Rect<int32_t> mExtent;
};
