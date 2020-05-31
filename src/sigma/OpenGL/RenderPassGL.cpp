#include <sigma/OpenGL/RenderPassGL.hpp>

const std::vector<RenderPassAttachment>& RenderPassGL::attachments() const
{
	return mAttachments;
}

bool RenderPassGL::initialize(const RenderPassCreateParams& inParams)
{
	mAttachments = inParams.attachments;
	return true;
}