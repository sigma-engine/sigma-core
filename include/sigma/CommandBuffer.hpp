#pragma once

#include <sigma/Rect.hpp>

#include <memory>

class RenderPass;
class Framebuffer;

struct RenderPassBeginParams {
    std::shared_ptr<RenderPass> renderPass;
    std::shared_ptr<Framebuffer> frameBuffer;
    Rect<int32_t> renderArea;
};

class CommandBuffer {
public:
    virtual ~CommandBuffer() = default;

    virtual void begin() = 0;

    virtual void beginRenderPass(const RenderPassBeginParams& inParams) = 0;

    virtual void endRenderPass() = 0;

    virtual void end() = 0;
};
