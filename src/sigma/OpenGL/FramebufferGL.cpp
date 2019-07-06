
#include <sigma/OpenGL/FramebufferGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>

FramebufferGL::FramebufferGL(std::shared_ptr<RenderPassGL> inRenderPass, Rect<int32_t> inExtent)
	: mRenderPass(inRenderPass)
	, mExtent(inExtent)
{
}

std::shared_ptr<RenderPass> FramebufferGL::renderPass() const
{
	return mRenderPass;
}

Rect<int32_t> FramebufferGL::extent() const
{
	return mExtent;
}
