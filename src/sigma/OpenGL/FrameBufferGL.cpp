#include <sigma/OpenGL/FrameBufferGL.hpp>

#include <sigma/OpenGL/RenderPassGL.hpp>
#include <sigma/OpenGL/TextureGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

FrameBufferGL::FrameBufferGL(std::shared_ptr<RenderPassGL> inRenderPass, const glm::uvec2& inSize, unsigned int inHandle)
	: mRenderPass(inRenderPass)
	, mSize(inSize)
	, mHandle(inHandle)
{
	const auto& attachmentDesc = mRenderPass->attachments();

	mDrawBuffers.resize(attachmentDesc.size());

	uint32_t colorAttachmentNumber = 0;
	for (size_t i = 0; i < attachmentDesc.size(); ++i) {
		switch (attachmentDesc[i].type) {
		case AttachmentType::ColorAttachment: {
			mDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + colorAttachmentNumber;
			colorAttachmentNumber++;
			break;
		}
		default: {
			// TODO: other attachment types
			SIGMA_ASSERT(false, "Attachment type not supported!");
			break;
		}
		}
	}
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
	SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassGL>(inParams.renderPass), "Must use opengl render pass with opengl framebuffer!");

	mSize = inParams.size;
	mRenderPass = std::static_pointer_cast<RenderPassGL>(inParams.renderPass);
	mAttachments.resize(inParams.attachments.size());
	mDrawBuffers.resize(inParams.attachments.size());

	const auto& attachmentDesc = mRenderPass->attachments();

	SIGMA_ASSERT(attachmentDesc.size() == inParams.attachments.size(), "FrameBuffer attachments must match render pass!");

	CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));

	uint32_t colorAttachmentNumber = 0;
	for (size_t i = 0; i < inParams.attachments.size(); ++i) {
		SIGMA_ASSERT(std::dynamic_pointer_cast<Texture2DGL>(inParams.attachments[i]), "Must use opengl texture with opengl framebuffer!");

		mAttachments[i] = std::static_pointer_cast<Texture2DGL>(inParams.attachments[i]);

		// TODO check format?

		switch (attachmentDesc[i].type) {
		case AttachmentType::ColorAttachment: {
			// TODO : texture types other than 2D?
			// TODO: non zero mip level?
			mDrawBuffers[i] = GL_COLOR_ATTACHMENT0 + colorAttachmentNumber;
			CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, mDrawBuffers[i], GL_TEXTURE_2D, mAttachments[i]->handle(), 0));
			colorAttachmentNumber++;
			break;
		}
		default: {
			SIGMA_ASSERT(false, "Attachment type not supported!");
			return false;
		}
		}
	}

	CHECK_GL(glDrawBuffers(static_cast<GLsizei>(mDrawBuffers.size()), mDrawBuffers.data()));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}

void FrameBufferGL::bind()
{
	CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
}