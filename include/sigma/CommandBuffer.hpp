#pragma once

#include <sigma/Rect.hpp>

#include <memory>

class RenderPass;
class FrameBuffer;
class Pipeline;
class VertexBuffer;
class IndexBuffer;
class DescriptorSet;

struct RenderPassBeginParams {
    std::shared_ptr<FrameBuffer> frameBuffer;
    Rect<int32_t> renderArea;
};

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;

    virtual void begin() = 0;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) = 0;

    virtual void bindPipeline(std::shared_ptr<Pipeline> inPipeline) = 0;

    virtual void bindDescriptorSet(std::shared_ptr<DescriptorSet> inDescriptorSet) = 0;

    virtual void bindVertexBuffer(std::shared_ptr<VertexBuffer> inBuffer) = 0;

    virtual void bindIndexBuffer(std::shared_ptr<IndexBuffer> inBuffer) = 0;

    virtual void draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance) = 0;

    virtual void drawIndexed(uint32_t inIndexCount, uint32_t inInstanceCount, uint32_t inFirstIndex, int32_t inVertexOffset, uint32_t inFirstInstance) = 0;

    virtual void endRenderPass() = 0;

    virtual void end() = 0;
};
