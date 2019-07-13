
#include <sigma/OpenGL/FrameBufferGL.hpp>

#include <sigma/OpenGL/RenderPassGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

FrameBufferGL::FrameBufferGL(std::shared_ptr<RenderPassGL> inRenderPass, const glm::uvec2& inSize, GLuint inHandle)
	: mRenderPass(inRenderPass)
	, mSize(inSize)
	, mHandle(inHandle)
{

}

FrameBufferGL::FrameBufferGL()
{
	CHECK_GL(glCreateFramebuffers(1, &mHandle));
}

FrameBufferGL::~FrameBufferGL()
{
	if (mHandle)
		CHECK_GL(glDeleteFramebuffers(1, &mHandle));
}

std::shared_ptr<RenderPass> FrameBufferGL::renderPass() const
{
    return mRenderPass;
}

glm::uvec2 FrameBufferGL::size() const
{
    return mSize;
}

bool FrameBufferGL::initialize(const FrameBufferCreateParams& inParams)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassGL>(inParams.renderPass), "Must use opengl render pass with opengl framebuffer");
    mSize = inParams.size;
    mRenderPass = std::static_pointer_cast<RenderPassGL>(inParams.renderPass);
    return false;
}