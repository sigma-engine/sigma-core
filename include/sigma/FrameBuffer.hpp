#pragma once

#include <sigma/Rect.hpp>

#include <memory>

class RenderPass;

struct FrameBufferCreateParams {
    glm::uvec2 size;
    std::shared_ptr<RenderPass> renderPass = nullptr;
};

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual std::shared_ptr<RenderPass> renderPass() const = 0;

    virtual glm::uvec2 size() const = 0;
};