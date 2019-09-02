#include <sigma/Vulkan/DeviceVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/CommandBufferVK.hpp>
#include <sigma/Vulkan/DescriptorSetVK.hpp>
#include <sigma/Vulkan/FrameBufferVK.hpp>
#include <sigma/Vulkan/IndexBufferVK.hpp>
#include <sigma/Vulkan/PipelineVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/SamplerVK.hpp>
#include <sigma/Vulkan/ShaderVK.hpp>
#include <sigma/Vulkan/SurfaceVK.hpp>
#include <sigma/Vulkan/TextureVK.hpp>
#include <sigma/Vulkan/UniformBufferVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>
#include <sigma/Vulkan/VertexBufferVK.hpp>

#include <fstream>

DeviceVK::DeviceVK(VkInstance inInstance, VkPhysicalDevice inDevice, const std::vector<std::string> &inEnabledLayers)
    : mInstance(inInstance), mPhysicalDevice(inDevice), mEnabledLayers(inEnabledLayers)
{
    vkGetPhysicalDeviceProperties(inDevice, &mPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(inDevice, &mPhysicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(inDevice, &mMemoryProperties);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);
    mQueueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, mQueueFamilyProperties.data());

    auto graphicsIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto &prop) {
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT);
    });
    if (graphicsIt != mQueueFamilyProperties.end())
        mGraphicsFamily = static_cast<uint32_t>(std::distance(mQueueFamilyProperties.begin(), graphicsIt));

    auto computeIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto &prop) {
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT);
    });
    if (computeIt != mQueueFamilyProperties.end())
        mComputeFamily = static_cast<uint32_t>(std::distance(mQueueFamilyProperties.begin(), computeIt));
}

DeviceVK::~DeviceVK()
{
    if (mDevice)
    {
        if (mDescriptorPool)
            vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);

        if (mGraphicsCommandPool)
            vkDestroyCommandPool(mDevice, mGraphicsCommandPool, nullptr);

        if (mAllocator)
            vmaDestroyAllocator(mAllocator);

        vkDestroyDevice(mDevice, nullptr);
    }
}

DeviceType DeviceVK::type() const
{
    DeviceType type = DeviceType::Unknown;
    switch (mPhysicalDeviceProperties.deviceType)
    {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    {
        type = DeviceType::DiscreteGPU;
        break;
    }
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    {
        type = DeviceType::IntegratedGPU;
        break;
    }
    default:
    {
        break;
    }
    }
    return type;
}

uint32_t DeviceVK::maxGraphicsQueues() const
{
    return mGraphicsFamily.has_value() ? mQueueFamilyProperties[mGraphicsFamily.value()].queueCount : 0;
}

uint32_t DeviceVK::maxComputeQueues() const
{
    return mComputeFamily.has_value() ? mQueueFamilyProperties[mComputeFamily.value()].queueCount : 0;
}

bool DeviceVK::supportsSurface(std::shared_ptr<Surface> inSurface) const
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<SurfaceVK>(inSurface), "Incorrect surface type!");
    auto vksurface = std::static_pointer_cast<SurfaceVK>(inSurface);
    return getSwapChainInfo(vksurface).has_value();
}

bool DeviceVK::initialize(const std::vector<std::shared_ptr<Surface>> &inSurfaces)
{
    static std::unordered_map<uint32_t, std::string> vendorNames = {
        {0x1002, "AMD"},
        {0x1010, "ImgTec"},
        {0x10DE, "NVIDIA"},
        {0x13B5, "ARM"},
        {0x5143, "Qualcomm"},
        {0x8086, "INTEL"}};
    SIGMA_INFO("Graphics API: Vulkan");
    SIGMA_INFO("Vender: {}", vendorNames.count(mPhysicalDeviceProperties.vendorID) ? vendorNames[mPhysicalDeviceProperties.vendorID] : "Unknown");
    SIGMA_INFO("Model: {}", mPhysicalDeviceProperties.deviceName);
    SIGMA_INFO("API Version: {}.{}.{}", (mPhysicalDeviceProperties.apiVersion >> 22) & 0x3FF, (mPhysicalDeviceProperties.apiVersion >> 12) & 0x3FF, mPhysicalDeviceProperties.apiVersion & 0xFFF);
    SIGMA_INFO("Driver Version: {}.{}.{}", (mPhysicalDeviceProperties.driverVersion >> 22) & 0x3FF, (mPhysicalDeviceProperties.driverVersion >> 12) & 0x3FF, mPhysicalDeviceProperties.driverVersion & 0xFFF);

    VkResult result;

    // TODO: add a way to specifiy required device extensions
    if (inSurfaces.size())
        mRequiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // Check if required extensions exists
    std::vector<VkExtensionProperties> extensionProperties;
    uint32_t extensionCount;
    CHECK_VK(result = vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, nullptr));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not enumerate extensions!");
        return false;
    }
    extensionProperties.resize(extensionCount);
    CHECK_VK(result = vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, extensionProperties.data()));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not enumerate extensions!");
        return false;
    }

    // Find and report about missing extensions
    auto extIt = std::remove_if(mRequiredExtensions.begin(), mRequiredExtensions.end(), [&](auto ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(extensionProperties.begin(), extensionProperties.end(), [&](const auto &prop) {
            return prop.extensionName == strName;
        });
        return it == extensionProperties.end();
    });

    for (auto it = extIt; it != mRequiredExtensions.end(); it++)
    {
        SIGMA_ERROR("Missing Required Vulkan Extension: {}", *it);
    }

    if (extIt != mRequiredExtensions.end())
    {
        SIGMA_INFO("Supported Vulkan Extensions: {}", fmt::join(extensionProperties.begin(), extensionProperties.end(), ","));
        return false;
    }

    // Check if layers exists
    std::vector<VkLayerProperties> layerProperties;
    uint32_t layerCount;
    CHECK_VK(result = vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, nullptr));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not enumerate device layers!");
        return false;
    }
    layerProperties.resize(layerCount);
    CHECK_VK(result = vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, layerProperties.data()));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not enumerate device layers!");
        return false;
    }

    // Find and report missing layers
    auto layerIt = std::remove_if(mEnabledLayers.begin(), mEnabledLayers.end(), [&](const auto &ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(layerProperties.begin(), layerProperties.end(), [&](const auto &prop) {
            return prop.layerName == strName;
        });
        return it == layerProperties.end();
    });

    for (auto it = layerIt; it != mEnabledLayers.end(); it++)
    {
        SIGMA_WARN("Skipping Missing Vulkan Layer: {}", *it);
    }

    if (layerIt != mEnabledLayers.end())
    {
        SIGMA_INFO("Supported Vulkan Layers: {}", fmt::join(layerProperties.begin(), layerProperties.end(), ","));
        mEnabledLayers.erase(layerIt, mEnabledLayers.end());
    }

    // TODO add a way to specifiy required queues
    std::set<uint32_t> queueFamilies;

    std::vector<SurfaceSwapChainInfoVK> surfaceSwapChainInfos;
    for (auto surface : inSurfaces)
    {
        SIGMA_ASSERT(std::dynamic_pointer_cast<SurfaceVK>(surface), "Incorrect surface type!");
        auto vksurface = std::static_pointer_cast<SurfaceVK>(surface);
        auto swapChainInfo = getSwapChainInfo(vksurface);
        if (!swapChainInfo.has_value() || !swapChainInfo->presentFamily.has_value())
        {
            SIGMA_ERROR("Devices does not support surface presentation!");
            return false;
        }

        surfaceSwapChainInfos.push_back(swapChainInfo.value());
        queueFamilies.insert(swapChainInfo->presentFamily.value());
    }

    if (mGraphicsFamily.has_value())
        queueFamilies.insert(mGraphicsFamily.value());

    if (mComputeFamily.has_value())
        queueFamilies.insert(mComputeFamily.value());

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1;
    for (auto family : queueFamilies)
    {
        queueCreateInfos.push_back({});
        queueCreateInfos.back().sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos.back().queueCount = 1;
        queueCreateInfos.back().pQueuePriorities = &queuePriority;
        queueCreateInfos.back().queueFamilyIndex = family;
    }

    // TODO fill out device features
    VkPhysicalDeviceFeatures deviceFeatures = {};

    std::vector<const char *> enabledLayers(mEnabledLayers.size());
    std::transform(mEnabledLayers.begin(), mEnabledLayers.end(), enabledLayers.begin(), [](const auto &ext) {
        return ext.c_str();
    });
    std::vector<const char *> enabledExtensions(mRequiredExtensions.size());
    std::transform(mRequiredExtensions.begin(), mRequiredExtensions.end(), enabledExtensions.begin(), [](const auto &ext) {
        return ext.c_str();
    });

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.ppEnabledLayerNames = enabledLayers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
    createInfo.ppEnabledExtensionNames = enabledExtensions.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());

    CHECK_VK(result = vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not create Vulkan logical device!");
        return false;
    }

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = mPhysicalDevice;
    allocatorInfo.device = mDevice;

    CHECK_VK(result = vmaCreateAllocator(&allocatorInfo, &mAllocator));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not create VulkanMemoryAllocator!");
        return false;
    }

    if (mGraphicsFamily.has_value())
    {
        VkCommandPoolCreateInfo graphicsPoolInfo = {};
        graphicsPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        graphicsPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        graphicsPoolInfo.queueFamilyIndex = mGraphicsFamily.value();

        if (vkCreateCommandPool(mDevice, &graphicsPoolInfo, nullptr, &mGraphicsCommandPool) != VK_SUCCESS)
        {
            SIGMA_ERROR("Could not create graphics command pool!");
            return false;
        }

        vkGetDeviceQueue(mDevice, mGraphicsFamily.value(), 0, &mGraphicsQueue);
    }

    for (std::size_t i = 0; i < inSurfaces.size(); ++i)
    {
        auto surface = std::static_pointer_cast<SurfaceVK>(inSurfaces[i]);
        if (!surface->createSwapChain(shared_from_this(), surfaceSwapChainInfos[i]))
        {
            SIGMA_ERROR("Could not create surface swapchain!");
            return false;
        }
    }

    // TODO other descriptor types?
    std::vector<VkDescriptorPoolSize> poolSizes(2);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 10; // TODO: Remove this hardcoded value
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = 10; // TODO: Remove this hardcoded value

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 10; // TODO: Remove this hardcoded value
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    CHECK_VK(result = vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool));
    if (result != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not create descriptor pool!");
        return false;
    }

    return true;
}

std::shared_ptr<CommandBuffer> DeviceVK::createCommandBuffer()
{
    if (mGraphicsCommandPool == nullptr)
        return nullptr;

    auto commandBuffer = std::make_shared<CommandBufferVK>(shared_from_this(), mGraphicsCommandPool);
    if (!commandBuffer->initialize())
        return nullptr;

    return commandBuffer;
}

std::shared_ptr<Shader> DeviceVK::createShader(ShaderType inType, const std::string &inSourcePath)
{
    std::ifstream file("vulkan/" + inSourcePath + ".glsl", std::ios::ate | std::ios::binary);
    if (!file.is_open())
        return nullptr;
    size_t fsize = file.tellg();
    std::vector<uint32_t> data(fsize / sizeof(uint32_t));
    file.seekg(0);
    file.read(reinterpret_cast<char *>(data.data()), fsize);

    std::shared_ptr<ShaderVK> shader = std::make_shared<ShaderVK>(inType, shared_from_this());
    if (!shader->initialize(data))
        return nullptr;

    return shader;
}

std::shared_ptr<RenderPass> DeviceVK::createRenderPass(const RenderPassCreateParams &inParams)
{
    auto renderPass = std::make_shared<RenderPassVK>(shared_from_this());
    if (!renderPass->initialize(inParams))
        return nullptr;

    return renderPass;
}

std::shared_ptr<FrameBuffer> DeviceVK::createFrameBuffer(const FrameBufferCreateParams &inParams)
{
    auto frameBuffer = std::make_shared<FrameBufferVK>(shared_from_this());
    if (!frameBuffer->initialize(inParams))
        return nullptr;

    return frameBuffer;
}

std::shared_ptr<DescriptorSetLayout> DeviceVK::createDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &inBindings)
{
    auto layout = std::make_shared<DescriptorSetLayoutVK>(shared_from_this());
    if (!layout->initialize(inBindings))
        return nullptr;

    return layout;
}

std::shared_ptr<DescriptorSet> DeviceVK::createDescriptorSet(const DescriptorSetCreateParams &inParams)
{
    auto set = std::make_shared<DescriptorSetVK>(shared_from_this(), mDescriptorPool);
    if (!set->initialize(inParams))
        return nullptr;

    return set;
}

std::shared_ptr<Pipeline> DeviceVK::createPipeline(const PipelineCreateParams &inParams)
{
    std::shared_ptr<PipelineVK> pipeline = std::make_shared<PipelineVK>(shared_from_this());
    if (!pipeline->initialize(inParams))
        return nullptr;

    return pipeline;
}

std::shared_ptr<VertexBuffer> DeviceVK::createVertexBuffer(const VertexLayout &inLayout, uint64_t inSize)
{
    auto vertexBuffer = std::make_shared<VertexBufferVK>(shared_from_this(), inLayout);
    if (!vertexBuffer->initialize(inSize))
        return nullptr;

    return vertexBuffer;
}

std::shared_ptr<IndexBuffer> DeviceVK::createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize)
{
    auto indexBuffer = std::make_shared<IndexBufferVK>(shared_from_this(), inPrimitive, inDataType);
    if (!indexBuffer->initialize(inSize))
        return nullptr;

    return indexBuffer;
}

std::shared_ptr<UniformBuffer> DeviceVK::createUniformBuffer(uint64_t inSize)
{
    auto buffer = std::make_shared<UniformBufferVK>(shared_from_this());
    if (!buffer->initialize(inSize))
        return nullptr;

    return buffer;
}

std::shared_ptr<Texture2D> DeviceVK::createTexture2D(const TextureCreateParams &inParams)
{
    auto texture = std::make_shared<Texture2DVK>(shared_from_this());
    if (!texture->initialize(inParams))
        return nullptr;

    return texture;
}

std::shared_ptr<Sampler2D> DeviceVK::createSampler2D(const SamplerCreateParams &inParams)
{
    auto sampler = std::make_shared<Sampler2DVK>(shared_from_this());
    if (!sampler->initialize(inParams))
        return nullptr;

    return sampler;
}

uint32_t DeviceVK::graphicsQueueFamily() const
{
    return mGraphicsFamily.value();
}

VkQueue DeviceVK::getQueue(uint32_t inFamily) const
{
    VkQueue queue = nullptr;
    vkGetDeviceQueue(mDevice, inFamily, 0, &queue);
    return queue;
}

VkQueue DeviceVK::graphicsQueue() const
{
    return mGraphicsQueue;
}

VkResult DeviceVK::startTmpCommandBuffer(VkCommandBuffer *outCommandBuffer)
{
    // TODO: this is crap
    VkResult result;
    VkCommandBufferAllocateInfo allocInfo = {};
    VkCommandBufferBeginInfo beginInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = mGraphicsCommandPool;
    allocInfo.commandBufferCount = 1;

    CHECK_VK(result = vkAllocateCommandBuffers(mDevice, &allocInfo, outCommandBuffer));
    if (result != VK_SUCCESS)
        goto done;

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    CHECK_VK(result = vkBeginCommandBuffer(*outCommandBuffer, &beginInfo));
    if (result != VK_SUCCESS)
        goto done;

done:
    return result;
}

VkResult DeviceVK::endTmpCommandBuffer(VkCommandBuffer inCommandBuffer)
{
    // TODO: this is crap
    VkResult result;
    VkSubmitInfo submitInfo = {};

    CHECK_VK(result = vkEndCommandBuffer(inCommandBuffer));
    if (result != VK_SUCCESS)
        goto done;

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &inCommandBuffer;

    CHECK_VK(result = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, nullptr));
    if (result != VK_SUCCESS)
        goto done;

    CHECK_VK(result = vkQueueWaitIdle(mGraphicsQueue));
done:
    if (inCommandBuffer)
        vkFreeCommandBuffers(mDevice, mGraphicsCommandPool, 1, &inCommandBuffer);

    return result;
}

#if 0
VkResult DeviceVK::createBuffer(VkBufferCreateInfo* inBufferCreateInfo, VkMemoryPropertyFlagBits inProperties, VkBuffer* outBuffer, VkDeviceMemory* outMemory)
{
    // TODO: this is crap (memory)
    VkResult result;
    VkMemoryRequirements requirements;
    VkMemoryAllocateInfo allocInfo = {};

    CHECK_VK(result = vkCreateBuffer(mDevice, inBufferCreateInfo, nullptr, outBuffer));
    if (result != VK_SUCCESS)
        goto done;

    vkGetBufferMemoryRequirements(mDevice, *outBuffer, &requirements);

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits, inProperties);

    CHECK_VK(result = vkAllocateMemory(mDevice, &allocInfo, nullptr, outMemory));
    if (result != VK_SUCCESS)
        goto done;

    CHECK_VK(result = vkBindBufferMemory(mDevice, *outBuffer, *outMemory, 0));
    if (result != VK_SUCCESS)
        goto done;

done:
    if (result != VK_SUCCESS) {
        if (*outMemory)
            vkFreeMemory(mDevice, *outMemory, nullptr);
        if (*outBuffer)
            vkDestroyBuffer(mDevice, *outBuffer, nullptr);
        *outBuffer = nullptr;
        *outMemory = nullptr;
    }
    return result;
}
#endif

VkResult DeviceVK::copyBufferToBuffer(VkBuffer inDstBuffer, VkBuffer inSrcBuffer, uint64_t inSize)
{
    // TODO: this is crap
    VkResult result;
    VkBufferCopy copyRegion = {0, 0, inSize};
    VkCommandBuffer commandBuffer = nullptr;

    CHECK_VK(result = startTmpCommandBuffer(&commandBuffer));
    if (result != VK_SUCCESS)
        goto done;

    vkCmdCopyBuffer(commandBuffer, inSrcBuffer, inDstBuffer, 1, &copyRegion);

    CHECK_VK(result = endTmpCommandBuffer(commandBuffer));
    if (result != VK_SUCCESS)
        goto done;
done:
    return result;
}

#if 0
VkResult DeviceVK::createImage(VkImageCreateInfo* inImageCreateInfo, VkMemoryPropertyFlagBits inProperties, VkImage* outImage, VkDeviceMemory* outMemory)
{
    // TODO: this is crap (memory)
    VkResult result;
    VkMemoryRequirements requirements;
    VkMemoryAllocateInfo allocInfo = {};

    CHECK_VK(result = vkCreateImage(mDevice, inImageCreateInfo, nullptr, outImage));
    if (result != VK_SUCCESS)
        goto done;

    vkGetImageMemoryRequirements(mDevice, *outImage, &requirements);

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits, inProperties);

    CHECK_VK(result = vkAllocateMemory(mDevice, &allocInfo, nullptr, outMemory));
    if (result != VK_SUCCESS)
        goto done;

    CHECK_VK(result = vkBindImageMemory(mDevice, *outImage, *outMemory, 0));
    if (result != VK_SUCCESS)
        goto done;

done:
    if (result != VK_SUCCESS) {
        if (*outMemory)
            vkFreeMemory(mDevice, *outMemory, nullptr);
        if (*outImage)
            vkDestroyImage(mDevice, *outImage, nullptr);
        *outImage = nullptr;
        *outMemory = nullptr;
    }
    return result;
}
#endif

void DeviceVK::transitionImageLayout(VkCommandBuffer inCommandBuffer, VkImage inImage, VkFormat inFormat, VkImageLayout inSrcLayout, VkImageLayout inDstLayout)
{
    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;
    VkImageMemoryBarrier barrier = {};

    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = inSrcLayout;
    barrier.newLayout = inDstLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = inImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    // TODO: add other layout transition types
    if (inSrcLayout == VK_IMAGE_LAYOUT_UNDEFINED && inDstLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (inSrcLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && inDstLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        SIGMA_ASSERT(false, "Unsupported layout transition!");
    }

    vkCmdPipelineBarrier(inCommandBuffer,
                         srcStage,
                         dstStage,
                         0,
                         0, nullptr,
                         0, nullptr,
                         1, &barrier);
}

VkResult DeviceVK::copyBufferToImage(VkImage inDstImage, VkBuffer inSrcBuffer, VkFormat inFormat, uint32_t inWidth, uint32_t inHeight)
{
    // TODO: this is crap
    VkResult result;
    VkCommandBuffer commandBuffer = nullptr;
    VkBufferImageCopy region = {};

    CHECK_VK(result = startTmpCommandBuffer(&commandBuffer));
    if (result != VK_SUCCESS)
        goto done;

    transitionImageLayout(commandBuffer, inDstImage, inFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {inWidth, inHeight, 1};

    vkCmdCopyBufferToImage(commandBuffer, inSrcBuffer, inDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    transitionImageLayout(commandBuffer, inDstImage, inFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    CHECK_VK(result = endTmpCommandBuffer(commandBuffer));
    if (result != VK_SUCCESS)
        goto done;
done:
    return result;
}

std::optional<SurfaceSwapChainInfoVK> DeviceVK::getSwapChainInfo(std::shared_ptr<SurfaceVK> inSurface) const
{
    SurfaceSwapChainInfoVK info;

    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, inSurface->handle(), &info.capabilities) != VK_SUCCESS)
        return {};

    uint32_t formatCount;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, inSurface->handle(), &formatCount, nullptr) != VK_SUCCESS || formatCount == 0)
        return {};

    info.formats.resize(formatCount);
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, inSurface->handle(), &formatCount, info.formats.data()) != VK_SUCCESS)
        return {};

    uint32_t modeCount;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, inSurface->handle(), &modeCount, nullptr) != VK_SUCCESS || modeCount == 0)
        return {};

    info.modes.resize(modeCount);
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice, inSurface->handle(), &modeCount, info.modes.data()) != VK_SUCCESS)
        return {};

    for (uint32_t i = 0; i < mQueueFamilyProperties.size(); ++i)
    {
        VkBool32 support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, i, inSurface->handle(), &support);
        if (support == VK_TRUE)
        {
            info.presentFamily = i;
            break;
        }
    }

    if (!info.presentFamily.has_value() || info.formats.empty() || info.modes.empty())
        return {};

    return std::move(info);
}

uint32_t DeviceVK::findMemoryType(uint32_t inTypeFilter, VkMemoryPropertyFlagBits inProperties) const
{
    for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; ++i)
    {
        if ((inTypeFilter & (1 << i)) && ((mMemoryProperties.memoryTypes[i].propertyFlags & inProperties) == inProperties))
        {
            return i;
        }
    }

    return std::numeric_limits<uint32_t>::max();
}