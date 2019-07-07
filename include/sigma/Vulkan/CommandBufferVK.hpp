#pragma once

#include <sigma/CommandBuffer.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class PipelineVK;
class VertexBufferVK;
class IndexBufferVK;
class DescriptorSetVK;

class CommandBufferVK : public CommandBuffer {
public:
    CommandBufferVK(std::shared_ptr<DeviceVK> inDevice, VkCommandPool inCommandPool);

    virtual ~CommandBufferVK();

    VkCommandBuffer handle() const { return mBuffer; }

    bool initialize();

    virtual void begin() override;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) override;

    virtual void bindPipeline(std::shared_ptr<Pipeline> inPipeline) override;

	virtual void bindDescriptorSet(std::shared_ptr<DescriptorSet> inDescriptorSet) override;

    virtual void bindVertexBuffer(std::shared_ptr<VertexBuffer> inBuffer) override;

    virtual void bindIndexBuffer(std::shared_ptr<IndexBuffer> inBuffer) override;

    virtual void draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance) override;

    virtual void drawIndexed(uint32_t inIndexCount, uint32_t inInstanceCount, uint32_t inFirstIndex, int32_t inVertexOffset, uint32_t inFirstInstance) override;

    virtual void endRenderPass() override;

    virtual void end() override;

public:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkCommandPool mCommandPool = nullptr;

    VkCommandBuffer mBuffer = nullptr;

    std::shared_ptr<PipelineVK> mCurrentPipeline = nullptr;
    std::shared_ptr<VertexBufferVK> mCurrentVertexBuffer = nullptr;
    std::shared_ptr<IndexBufferVK> mCurrentIndexBuffer = nullptr;
	std::shared_ptr<DescriptorSetVK> mCurrentDescriptorSet = nullptr;
};
