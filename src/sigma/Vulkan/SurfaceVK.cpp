#include <sigma/Vulkan/SurfaceVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Shader.hpp>
#include <sigma/Vulkan/CommandBufferVK.hpp>
#include <sigma/Vulkan/DeviceManagerVK.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/FramebufferVK.hpp>
#include <sigma/Vulkan/PipelineVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

#include <numeric>

SurfaceVK::~SurfaceVK()
{
    if (mDevice) {
        vkWaitForFences(mDevice->handle(), static_cast<uint32_t>(mFrameFences.size()), mFrameFences.data(), VK_TRUE, std::numeric_limits<uint64_t>::max());

        for (auto fence : mFrameFences)
            vkDestroyFence(mDevice->handle(), fence, nullptr);

        for (auto semaphore : mImageAvailableSemaphores)
            vkDestroySemaphore(mDevice->handle(), semaphore, nullptr);

        for (auto semaphore : mRenderFinishedSemaphores)
            vkDestroySemaphore(mDevice->handle(), semaphore, nullptr);

        mFrameData.clear();

        for (std::size_t i = 0; i < mImageViews.size(); ++i) {
            if (mImageViews[i])
                vkDestroyImageView(mDevice->handle(), mImageViews[i], nullptr);
        }

        if (mSwapChain) {
            vkDestroySwapchainKHR(mDevice->handle(), mSwapChain, nullptr);
        }
    }

    if (mSurface && mInstance) {
        vkDestroySurfaceKHR(mInstance->handle(), mSurface, nullptr);
    }
}

glm::uvec2 SurfaceVK::size() const
{
    return { mWidth, mHeight };
}

ImageFormat SurfaceVK::format() const
{
    return convertImageFormatVK(mSurfaceFormat.format);
}

uint32_t SurfaceVK::imageCount() const
{
	return static_cast<uint32_t>(mImages.size());
}

std::shared_ptr<RenderPass> SurfaceVK::renderPass() const
{
    return mRenderPass;
}

void SurfaceVK::nextFrame(SurfaceFrameData*& outData)
{
	uint32_t imageIndex;
	uint32_t frameIndex = mCurrentFrameIndex;
	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mMaxPendingFrames;

    vkWaitForFences(mDevice->handle(), 1, &mFrameFences[frameIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(mDevice->handle(), 1, &mFrameFences[frameIndex]);

    vkAcquireNextImageKHR(mDevice->handle(), mSwapChain, std::numeric_limits<uint64_t>::max(), mImageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, &imageIndex);
	outData = mFrameData.data() + imageIndex;
	outData->commandBuffers.clear();
	outData->imageIndex = imageIndex;
	outData->frameIndex = frameIndex;
}

void SurfaceVK::presentFrame(const SurfaceFrameData* inData)
{
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { mImageAvailableSemaphores[inData->frameIndex] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	if (mFrameData[inData->imageIndex].vkCommandBuffers.size() < inData->commandBuffers.size())
		mFrameData[inData->imageIndex].vkCommandBuffers.resize(inData->commandBuffers.size());

	for (size_t i = 0; i < inData->commandBuffers.size(); ++i)
	{
		// TODO : check command buffers are really vulkan command buffers
		mFrameData[inData->imageIndex].vkCommandBuffers[i] = std::static_pointer_cast<CommandBufferVK>(inData->commandBuffers[i])->handle();
	}

	submitInfo.commandBufferCount = static_cast<uint32_t>(inData->commandBuffers.size());
    submitInfo.pCommandBuffers = mFrameData[inData->imageIndex].vkCommandBuffers.data();

	VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphores[inData->frameIndex] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // TODO check for errors
    vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mFrameFences[inData->frameIndex]);

    VkSwapchainKHR swapChains[] = { mSwapChain };
    VkPresentInfoKHR presetInfo = {};
    presetInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presetInfo.waitSemaphoreCount = 1;
    presetInfo.pWaitSemaphores = signalSemaphores;
    presetInfo.swapchainCount = 1;
    presetInfo.pSwapchains = swapChains;
    presetInfo.pImageIndices = &inData->imageIndex;
    presetInfo.pResults = nullptr;

    vkQueuePresentKHR(mPresetQueue, &presetInfo);
}

bool SurfaceVK::createSwapChain(std::shared_ptr<DeviceVK> inDevice, const SurfaceSwapChainInfoVK& inInfo)
{
    mDevice = inDevice;
    mExtent = chooseSwapExtent(inInfo.capabilities);
    mSurfaceFormat = chooseSurfaceFormat(inInfo.formats);
    mPresentMode = choosePresentMode(inInfo.modes);
    mPresetQueue = mDevice->getQueue(inInfo.presentFamily.value());
    mGraphicsQueue = mDevice->graphicsQueue();
    uint32_t imageCount = std::max(inInfo.capabilities.minImageCount, 2u);
    if (inInfo.capabilities.maxImageCount > 0)
        imageCount = std::min(imageCount, inInfo.capabilities.maxImageCount);

    RenderPassCreateParams renderPassCreateParams = {
        { { AttachmentType::ColorAttachment, format() } }
    };
    mRenderPass = std::static_pointer_cast<RenderPassVK>(inDevice->createRenderPass(renderPassCreateParams));
    if (!mRenderPass)
        return false;

    uint32_t queueFamilyIndices[] = { inDevice->graphicsQueueFamily(), inInfo.presentFamily.value() };
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = mSurfaceFormat.format;
    createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
    createInfo.imageExtent = mExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = inInfo.capabilities.currentTransform;
    if (inInfo.capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    else if (inInfo.capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
    else if (inInfo.capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR;
    else
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    createInfo.presentMode = mPresentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(mDevice->handle(), &createInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create Swapchain for surface!");
        return false;
    }

    if (vkGetSwapchainImagesKHR(mDevice->handle(), mSwapChain, &imageCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not get Swapchain images!");
        return false;
    }
    mImages.resize(imageCount);
    if (vkGetSwapchainImagesKHR(mDevice->handle(), mSwapChain, &imageCount, mImages.data()) != VK_SUCCESS) {
        SIGMA_ERROR("Could not get Swapchain images!");
        return false;
    }

    mImageViews.resize(imageCount);
    mFrameData.resize(imageCount);
    for (size_t i = 0; i < mImageViews.size(); ++i) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = mImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = mSurfaceFormat.format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(mDevice->handle(), &createInfo, nullptr, &mImageViews[i]) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create Swapchain image view!");
            return false;
        }

        VkImageView attachments[] = {
            mImageViews[i]
        };

        VkFramebufferCreateInfo frameBufferInfo = {};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = mRenderPass->handle();
        frameBufferInfo.attachmentCount = 1;
        frameBufferInfo.pAttachments = attachments;
        frameBufferInfo.width = mExtent.width;
        frameBufferInfo.height = mExtent.height;
        frameBufferInfo.layers = 1;

        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(mDevice->handle(), &frameBufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create Swapchain frame buffer!");
            return false;
        }
		mFrameData[i].framebuffer = std::make_shared<FramebufferVK>(mDevice, framebuffer, mRenderPass, Rect<int32_t>{ { 0, 0 }, { mExtent.width, mExtent.height } });
    }

    mImageAvailableSemaphores.resize(mMaxPendingFrames);
    mRenderFinishedSemaphores.resize(mMaxPendingFrames);
    mFrameFences.resize(mMaxPendingFrames);
    for (uint32_t i = 0; i < mMaxPendingFrames; ++i) {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(mDevice->handle(), &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create semaphore!");
            return false;
        }

        if (vkCreateSemaphore(mDevice->handle(), &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create semaphore!");
            return false;
        }

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        if (vkCreateFence(mDevice->handle(), &fenceInfo, nullptr, &mFrameFences[i]) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create fence!");
            return false;
        }
    }

    return true;
}

VkSurfaceFormatKHR SurfaceVK::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& inFormats) const
{
    if (inFormats.size() == 1 && inFormats[1].format == VK_FORMAT_UNDEFINED) {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& format : inFormats) {
        if (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && format.format == VK_FORMAT_B8G8R8A8_UNORM) {
            return format;
        }
    }

    return inFormats[0];
}

VkPresentModeKHR SurfaceVK::choosePresentMode(const std::vector<VkPresentModeKHR>& inModes) const
{
    // TODO: for now FIFO is fine and guaranteed by the spec.
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SurfaceVK::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& inCapabilities) const
{
    if (inCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return inCapabilities.currentExtent;
    } else {
        VkExtent2D extent;
        extent.width = std::max(inCapabilities.minImageExtent.width, std::min(inCapabilities.maxImageExtent.width, mWidth));
        extent.height = std::max(inCapabilities.minImageExtent.height, std::min(inCapabilities.maxImageExtent.height, mHeight));
        return extent;
    }
}
