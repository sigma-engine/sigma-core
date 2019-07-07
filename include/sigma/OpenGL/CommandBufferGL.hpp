#pragma once

#include <sigma/CommandBuffer.hpp>

#include <glad/glad.h>

class PipelineGL;
class DescriptorSetGL;
class VertexBufferGL;
class IndexBufferGL;

class CommandBufferGL : public CommandBuffer {
public:
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

private:
    GLenum mBoundIndexType = GL_INVALID_ENUM;

	std::shared_ptr<PipelineGL> mPipeline = nullptr;
	std::shared_ptr<DescriptorSetGL> mDescriptorSet = nullptr;
	std::shared_ptr<VertexBufferGL> mVertexBuffer = nullptr;
	std::shared_ptr<IndexBufferGL> mIndexBuffer = nullptr;
};
