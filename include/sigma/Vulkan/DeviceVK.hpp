#pragma once

#include <sigma/Device.hpp>

#include <sigma/Vulkan/DeviceManagerVK.hpp>

#include <vulkan/vulkan.h>

#include <optional>
#include <set>
#include <string>

class SurfaceVK;

struct SurfaceSwapChainInfoVK {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> modes;
    std::optional<uint32_t> presentFamily;
};

class DeviceVK : public Device, public std::enable_shared_from_this<DeviceVK> {
public:
    DeviceVK(VkInstance inInstance, VkPhysicalDevice inDevice, const std::vector<std::string>& inEnabledLayers);

    virtual ~DeviceVK();

    virtual DeviceType type() const override;

    virtual uint32_t maxGraphicsQueues() const override;

    virtual uint32_t maxComputeQueues() const override;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const override;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces) override;

    virtual std::shared_ptr<CommandBuffer> createCommandBuffer() override;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string& inSourcePath) override;

    virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPassCreateParams& inParams) override;

    virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& inBindings) override;

    virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSetCreateParams &inParams) override;

    virtual std::shared_ptr<Pipeline> createPipeline(const PipelineCreateParams& inParams) override;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexLayout& inLayout, uint64_t inSize) override;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize) override;

    virtual std::shared_ptr<UniformBuffer> createUniformBuffer(uint64_t inSize) override;

    uint32_t graphicsQueueFamily() const;

    VkDevice handle() const { return mDevice; }

    VkQueue getQueue(uint32_t inFamily) const;

    VkQueue graphicsQueue() const;

    VkResult createBuffer(VkBufferCreateInfo* inBufferCreateInfo, VkMemoryPropertyFlagBits inProperties, VkBuffer* outBuffer, VkDeviceMemory* outMemory);

    VkResult copyBuffer(VkBuffer inDstBuffer, VkBuffer inSrcBuffer, uint64_t inSize);

private:
    VkInstance mInstance = nullptr;
    VkPhysicalDevice mPhysicalDevice = nullptr;
    VkDevice mDevice = nullptr;
    std::vector<std::string> mRequiredExtensions;
    std::vector<std::string> mEnabledLayers;
    VkPhysicalDeviceProperties mPhysicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties mMemoryProperties;
    std::vector<VkExtensionProperties> mExtensionProperties;
    std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
    VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
    std::optional<uint32_t> mGraphicsFamily;
    std::optional<uint32_t> mComputeFamily;
    VkQueue mGraphicsQueue = nullptr;

    VkCommandPool mGraphicsCommandPool = nullptr;
    VkDescriptorPool mDescriptorPool = nullptr;
    std::vector<VkCommandBuffer> mFreeGraphicsBuffers;

    std::optional<SurfaceSwapChainInfoVK> getSwapChainInfo(std::shared_ptr<SurfaceVK> inSurface) const;

    uint32_t findMemoryType(uint32_t inTypeFilter, VkMemoryPropertyFlagBits inProperties) const;
};
