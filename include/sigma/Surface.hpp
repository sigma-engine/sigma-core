#pragma once

#include <memory>

#include <glm/vec2.hpp>

class DeviceManager;
enum class ImageFormat;
class RenderPass;
class CommandBuffer;

struct SurfaceFrameData {
	uint32_t imageIndex;
	uint32_t frameIndex;
	std::shared_ptr<CommandBuffer> commandBuffer;
};

class Surface {
public:
    virtual ~Surface() = default;

    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) = 0;

    virtual glm::uvec2 size() const = 0;

    virtual ImageFormat format() const = 0;

    virtual std::shared_ptr<RenderPass> renderPass() const = 0;

    virtual void beginFrame(SurfaceFrameData &outData) = 0;

    virtual void endFrame(const SurfaceFrameData &inData) = 0;
};
