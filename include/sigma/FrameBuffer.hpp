#pragma once

#include <sigma/Rect.hpp>

#include <memory>

class RenderPass;

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual std::shared_ptr<RenderPass> renderPass() const = 0;

    virtual Rect<int32_t> extent() const = 0;
};