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
        if (mDevice)
            vkDeviceWaitIdle(mDevice->handle());

        if (mImageAvailableSemaphore)
            vkDestroySemaphore(mDevice->handle(), mImageAvailableSemaphore, nullptr);

        if (mRenderFinishedSemaphore)
            vkDestroySemaphore(mDevice->handle(), mRenderFinishedSemaphore, nullptr);

        mCommandBuffers.clear();
        mFramebuffers.clear();

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

std::shared_ptr<RenderPass> SurfaceVK::renderPass() const
{
    return mRenderPass;
}

std::shared_ptr<CommandBuffer> SurfaceVK::beginFrame()
{
    vkAcquireNextImageKHR(mDevice->handle(), mSwapChain, std::numeric_limits<uint64_t>::max(), mImageAvailableSemaphore, VK_NULL_HANDLE, &mCurrentFrameIndex);
    auto commandBuffer = mCommandBuffers[mCurrentFrameIndex];
    RenderPassBeginParams beginRenderPass {
        mRenderPass,
        mFramebuffers[mCurrentFrameIndex],
        { { 0, 0 }, { mExtent.width, mExtent.height } }
    };

    mCommandBuffers[mCurrentFrameIndex]->begin();
    mCommandBuffers[mCurrentFrameIndex]->beginRenderPass(beginRenderPass);

    vkCmdBindPipeline(mCommandBuffers[mCurrentFrameIndex]->handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline->handle());

    vkCmdDraw(mCommandBuffers[mCurrentFrameIndex]->handle(), 3, 1, 0, 0);

    mCommandBuffers[mCurrentFrameIndex]->endRenderPass();
    mCommandBuffers[mCurrentFrameIndex]->end();

    return commandBuffer;
}

void SurfaceVK::endFrame()
{
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { mImageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkCommandBuffer commandBuffers[] = { mCommandBuffers[mCurrentFrameIndex]->handle() };
    VkSemaphore signalSemaphores[] = { mRenderFinishedSemaphore };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffers;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // TODO check for errors
    vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mCommandBuffers[mCurrentFrameIndex]->mFence);

    VkSwapchainKHR swapChains[] = { mSwapChain };
    VkPresentInfoKHR presetInfo = {};
    presetInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presetInfo.waitSemaphoreCount = 1;
    presetInfo.pWaitSemaphores = signalSemaphores;
    presetInfo.swapchainCount = 1;
    presetInfo.pSwapchains = swapChains;
    presetInfo.pImageIndices = &mCurrentFrameIndex;
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
    uint32_t imageCount = inInfo.capabilities.minImageCount + 1;
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
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
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
    mFramebuffers.resize(imageCount);
    mCommandBuffers.resize(imageCount);
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
        mFramebuffers[i] = std::make_shared<FramebufferVK>(mDevice, framebuffer);

        mCommandBuffers[i] = std::static_pointer_cast<CommandBufferVK>(mDevice->createCommandBuffer());
        if (mCommandBuffers[i] == nullptr) {
            SIGMA_ERROR("Could not create command buffer!");
            return false;
        }
    }

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(mDevice->handle(), &semaphoreInfo, nullptr, &mImageAvailableSemaphore) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create semaphore!");
        return false;
    }

    if (vkCreateSemaphore(mDevice->handle(), &semaphoreInfo, nullptr, &mRenderFinishedSemaphore) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create semaphore!");
        return false;
    }

    auto vertexShader = mDevice->createShader(ShaderType::VertexShader, "shaders/simple.vert");
    if (!vertexShader)
        return false;

    auto fragmentShader = mDevice->createShader(ShaderType::FragmentShader, "shaders/simple.frag");
    if (!fragmentShader)
        return false;

    PipelineCreateParams pipelineParams = {
        { { 0, 0 }, { mExtent.width, mExtent.height } },
        mRenderPass,
        { vertexShader, fragmentShader }
    };
    mPipeline = std::static_pointer_cast<PipelineVK>(mDevice->createPipeline(pipelineParams));
    if (!mPipeline)
        return false;

    return true;
}

VkSurfaceFormatKHR SurfaceVK::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& inFormats) const
{
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
