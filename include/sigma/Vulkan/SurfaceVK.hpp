#pragma once

#include <sigma/Surface.hpp>

#include <vulkan/vulkan.h>

#include <vector>

class DeviceVK;
class DeviceManagerVK;
class SurfaceSwapChainInfoVK;

class SurfaceVK : public Surface {
public:
    virtual ~SurfaceVK();

    VkSurfaceKHR handle() { return mSurface; }

    bool createSwapChain(std::shared_ptr<DeviceVK> inDevice, const SurfaceSwapChainInfoVK &inInfo);
protected:
    std::shared_ptr<DeviceManagerVK> mInstance = nullptr;
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    uint32_t mWidth;
    uint32_t mHeight;
    VkSurfaceKHR mSurface = nullptr;

    VkExtent2D mExtent;
    VkSurfaceFormatKHR mSurfaceFormat;
    VkPresentModeKHR mPresentMode;
    uint32_t mImageCount;
    VkSwapchainKHR mSwapChain;
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;

    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &inFormats) const;

    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> &inModes) const;

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &inCapabilities) const;
};
