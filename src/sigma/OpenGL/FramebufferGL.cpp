
#include <sigma/OpenGL/FrameBufferGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>

FrameBufferGL::FrameBufferGL(std::shared_ptr<RenderPassGL> inRenderPass, Rect<int32_t> inExtent)
	: mRenderPass(inRenderPass)
	, mExtent(inExtent)
{
}

std::shared_ptr<RenderPass> FrameBufferGL::renderPass() const
{
	return mRenderPass;
}

Rect<int32_t> FrameBufferGL::extent() const
{
	return mExtent;
}
