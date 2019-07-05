#pragma once

#include <sigma/CommandBuffer.hpp>

class CommandBufferGL : public CommandBuffer {
public:
    virtual void begin() override;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) override;

	virtual void bindPipeline(std::shared_ptr<Pipeline> inPipeline) override;

	virtual void draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance) override;

    virtual void endRenderPass() override;

    virtual void end() override;
};
