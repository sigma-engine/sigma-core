#pragma once

#include <sigma/CommandBuffer.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class CommandBufferVK : public CommandBuffer {
public:
    CommandBufferVK(std::shared_ptr<DeviceVK> inDevice, VkCommandPool inCommandPool);

    virtual ~CommandBufferVK();

    VkCommandBuffer handle() const { return mBuffer; }

    bool initialize();

    virtual void begin() override;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) override;

    virtual void endRenderPass() override;

    virtual void end() override;

public:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkCommandPool mCommandPool = nullptr;

    bool mInit = false;

    VkCommandBuffer mBuffer = nullptr;
    VkFence mFence = nullptr;
};
