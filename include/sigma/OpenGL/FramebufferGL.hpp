#pragma once

#include <sigma/Framebuffer.hpp>

#include <memory>

class DeviceGL;
class RenderPassGL;

class FramebufferGL : public Framebuffer {
public:
	FramebufferGL(std::shared_ptr<RenderPassGL> inRenderPass, Rect<int32_t> inExtent);

	std::shared_ptr<RenderPass> renderPass() const override;

	virtual Rect<int32_t> extent() const override;

private:
	std::shared_ptr<RenderPassGL> mRenderPass = nullptr;
	Rect<int32_t> mExtent;
};