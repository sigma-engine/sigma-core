#pragma once

#include <sigma/CommandBuffer.hpp>

class CommandBufferGL : public CommandBuffer {
public:
    virtual void begin() override;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) override;

    virtual void endRenderPass() override;

    virtual void end() override;
};
