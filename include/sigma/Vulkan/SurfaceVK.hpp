#pragma once

#include <sigma/Surface.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <memory>

class DeviceVK;
class DeviceManagerVK;
struct SurfaceSwapChainInfoVK;
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

    virtual void beginFrame(SurfaceFrameData &outData) override;

    virtual void endFrame(const SurfaceFrameData &inData) override;

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

	uint32_t mMaxPendingFrames = 2;
	uint32_t mCurrentFrameIndex = 0;
    std::vector<VkSemaphore> mImageAvailableSemaphores;
    std::vector<VkSemaphore> mRenderFinishedSemaphores;
	std::vector<VkFence> mFrameFences;

    VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& inFormats) const;

    VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& inModes) const;

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& inCapabilities) const;
};
