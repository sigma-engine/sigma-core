#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

class DeviceManager;
enum class ImageFormat;
class RenderPass;
class CommandBuffer;
class Framebuffer;

struct SurfaceFrameData {
    uint32_t imageIndex;
    uint32_t frameIndex;
	std::shared_ptr<Framebuffer> framebuffer;
    std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;
};

class Surface {
public:
    virtual ~Surface() = default;

    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) = 0;

    virtual glm::uvec2 size() const = 0;

    virtual ImageFormat format() const = 0;

	virtual uint32_t imageCount() const = 0;

    virtual std::shared_ptr<RenderPass> renderPass() const = 0;

    virtual void nextFrame(SurfaceFrameData*& outData) = 0;

    virtual void presentFrame(const SurfaceFrameData* inData) = 0;
};
