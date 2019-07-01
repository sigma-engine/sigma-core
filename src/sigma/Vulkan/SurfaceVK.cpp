#include <sigma/Vulkan/SurfaceVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceManagerVK.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

SurfaceVK::~SurfaceVK()
{
    if (mDevice)
    {
        for(auto imageView: mImageViews)
        {
            if (imageView)
                vkDestroyImageView(mDevice->handle(), imageView, nullptr);
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
    return {mWidth, mHeight};
}

ImageFormat SurfaceVK::format() const
{
    return convertImageFormatVK(mSurfaceFormat.format);
}

bool SurfaceVK::createSwapChain(std::shared_ptr<DeviceVK> inDevice, const SurfaceSwapChainInfoVK &inInfo)
{
    mDevice = inDevice;
    mExtent = chooseSwapExtent(inInfo.capabilities);
    mSurfaceFormat = chooseSurfaceFormat(inInfo.formats);
    mPresentMode = choosePresentMode(inInfo.modes);
    mImageCount = inInfo.capabilities.minImageCount + 1;
    if (inInfo.capabilities.maxImageCount > 0)
        mImageCount = std::min(mImageCount, inInfo.capabilities.maxImageCount);

    uint32_t queueFamilyIndices[] = {inDevice->graphicsQueueFamily(), inInfo.presentFamily.value()};
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mSurface;
    createInfo.minImageCount = mImageCount;
    createInfo.imageFormat = mSurfaceFormat.format;
    createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
    createInfo.imageExtent = mExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }
    createInfo.preTransform = inInfo.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = mPresentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(mDevice->handle(), &createInfo, nullptr, &mSwapChain) != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not create Swapchain for surface!");
        return false;
    }

    uint32_t imageCount;
    if (vkGetSwapchainImagesKHR(mDevice->handle(), mSwapChain, &imageCount, nullptr) != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not get Swapchain images!");
        return false;
    }
    mImages.resize(imageCount);
    if (vkGetSwapchainImagesKHR(mDevice->handle(), mSwapChain, &imageCount, mImages.data()) != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not get Swapchain images!");
        return false;
    }

    mImageViews.resize(imageCount);


    for(size_t i = 0; i < mImageViews.size(); ++i)
    {
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
        if (vkCreateImageView(mDevice->handle(), &createInfo, nullptr, &mImageViews[i]) != VK_SUCCESS)
        {
            SIGMA_ERROR("Could not create Swapchain image view!");
            return false;
        }
    }

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

VkExtent2D SurfaceVK::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &inCapabilities) const
{
    if (inCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return inCapabilities.currentExtent;
    }
    else
    {
        VkExtent2D extent;
        extent.width = std::max(inCapabilities.minImageExtent.width, std::min(inCapabilities.maxImageExtent.width, mWidth));
        extent.height = std::max(inCapabilities.minImageExtent.height, std::min(inCapabilities.maxImageExtent.height, mHeight));
        return extent;
    }
}
