#include <sigma/Vulkan/FrameBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>
#include <sigma/Vulkan/TextureVK.hpp>

FrameBufferVK::FrameBufferVK(std::shared_ptr<DeviceVK> inDecice)
	: mDevice(inDecice)
{

}

FrameBufferVK::FrameBufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFrameBuffer, std::shared_ptr<RenderPassVK> inRenderPass, const glm::uvec2& inSize)
    : mDevice(inDevice)
    , mFrameBuffer(inFrameBuffer)
    , mRenderPass(inRenderPass)
    , mSize(inSize)
{
}

FrameBufferVK::~FrameBufferVK()
{
    if (mDevice && mFrameBuffer)
        vkDestroyFramebuffer(mDevice->handle(), mFrameBuffer, nullptr);
}

std::shared_ptr<RenderPass> FrameBufferVK::renderPass() const
{
    return mRenderPass;
}

glm::uvec2 FrameBufferVK::size() const
{
    return mSize;
}

bool FrameBufferVK::initialize(const FrameBufferCreateParams &inParams)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassVK>(inParams.renderPass), "Must use vulkan renderpass with vulkan framebuffer!");

	mSize = inParams.size;
	mRenderPass = std::static_pointer_cast<RenderPassVK>(inParams.renderPass);
	mAttachments.resize(inParams.attachments.size());

	const auto &attachmentDesc = mRenderPass->attachments();

	SIGMA_ASSERT(attachmentDesc.size() == inParams.attachments.size(), "FrameBuffer attachments must match RenderPass!");

	std::vector<VkImageView> attachmentViews(attachmentDesc.size());
	for (size_t i = 0; i < attachmentDesc.size(); ++i)
	{
		SIGMA_ASSERT(std::dynamic_pointer_cast<Texture2DVK>(inParams.attachments[i]), "Must use vulkan texture with vulkan framebuffer!");

		mAttachments[i] = std::static_pointer_cast<Texture2DVK>(inParams.attachments[i]);
		attachmentViews[i] = mAttachments[i]->imageViewHandle();
	}

	VkResult result;
	VkFramebufferCreateInfo frameBufferInfo = {};
	frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferInfo.renderPass = mRenderPass->handle();
	frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachmentViews.size());
	frameBufferInfo.pAttachments = attachmentViews.data();
	frameBufferInfo.width = mSize.x;
	frameBufferInfo.height = mSize.y;
	frameBufferInfo.layers = 1;

	CHECK_VK(result = vkCreateFramebuffer(mDevice->handle(), &frameBufferInfo, nullptr, &mFrameBuffer));
	return result == VK_SUCCESS;
}