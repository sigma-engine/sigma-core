#pragma once

#include <sigma/RenderPass.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;

class RenderPassVK : public RenderPass {
public:
    RenderPassVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~RenderPassVK();

    virtual const std::vector<RenderPassAttachment>& attachments() const override;

    VkRenderPass handle() const { return mRenderPass; }

    bool initialize(const RenderPassCreateParams& inParams);

private:
    std::vector<RenderPassAttachment> mAttachments;

    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkRenderPass mRenderPass = nullptr;
};
