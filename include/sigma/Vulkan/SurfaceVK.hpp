#pragma once

#include <sigma/Surface.hpp>

#include <vulkan/vulkan.h>

#include <vector>

class DeviceVK;
class DeviceManagerVK;
class SurfaceSwapChainInfoVK;
class RenderPassVK;
class CommandBufferVK;
class FramebufferVK;
class PipelineVK;

class Pipeline;

class SurfaceVK : public Surface {
public:
    virtual ~SurfaceVK();

    virtual glm::uvec2 size() const override;

    virtual ImageFormat format() const override;

    virtual std::shared_ptr<RenderPass> renderPass() const override;

    virtual std::shared_ptr<CommandBuffer> beginFrame() override;

    virtual void endFrame() override;

    VkSurfaceKHR handle() { return mSurface; }

    bool createSwapChain(std::shared_ptr<DeviceVK> inDevice, const SurfaceSwapChainInfoVK& inInfo);

protected:
    std::shared_ptr<DeviceManagerVK> mInstance = nullptr;
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    uint32_t mWidth;
    uint32_t mHeight;
    VkSurfaceKHR mSurface = nullptr;

    VkQueue mPresetQueue = nullptr;
    VkQueue mGraphicsQueue = nullptr;
    VkExtent2D mExtent;
    VkSurfaceFormatKHR mSurfaceFormat;
    VkPresentModeKHR mPresentMode;

    VkSwapchainKHR mSwapChain;
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;
    std::shared_ptr<RenderPassVK> mRenderPass = nullptr;
    std::vector<std::shared_ptr<FramebufferVK>> mFramebuffers;
    std::vector<std::shared_ptr<CommandBufferVK>> mCommandBuffers;

    uint32_t mCurrentFrameIndex;
    VkSemaphore mImageAvailableSemaphore = nullptr;
    VkSemaphore mRenderFinishedSemaphore = nullptr;

    std::shared_ptr<PipelineVK> mPipeline = nullptr;
    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& inFormats) const;

    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& inModes) const;

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& inCapabilities) const;
};
