#pragma once

#include <sigma/Surface.hpp>

#include <glad/glad.h>

class DeviceGL;
class RenderPassGL;
class CommandBufferGL;

class SurfaceGL : public Surface {
public:
    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) override;

    virtual glm::uvec2 size() const override;

    virtual ImageFormat format() const override;

    virtual std::shared_ptr<RenderPass> renderPass() const override;

    virtual void beginFrame(SurfaceImageData &outData) override;

    bool createSwapChain(std::shared_ptr<DeviceGL> inDevice);

protected:
    uint32_t mWidth;
    uint32_t mHeight;
    GLenum mFormat;

    std::shared_ptr<RenderPassGL> mRenderPass = nullptr;
	std::shared_ptr<CommandBufferGL> mCommandBuffer = nullptr;
};
