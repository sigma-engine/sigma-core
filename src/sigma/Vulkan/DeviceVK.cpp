#include <sigma/Vulkan/DeviceVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/CommandBufferVK.hpp>
#include <sigma/Vulkan/IndexBufferVK.hpp>
#include <sigma/Vulkan/PipelineVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/ShaderVK.hpp>
#include <sigma/Vulkan/SurfaceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>
#include <sigma/Vulkan/VertexBufferVK.hpp>

#include <sigma/Log.hpp>

#include <fstream>

DeviceVK::DeviceVK(VkInstance inInstance, VkPhysicalDevice inDevice, const std::vector<std::string>& inEnabledLayers)
    : mInstance(inInstance)
    , mPhysicalDevice(inDevice)
    , mEnabledLayers(inEnabledLayers)
{
    vkGetPhysicalDeviceProperties(inDevice, &mPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(inDevice, &mPhysicalDeviceFeatures);
    vkGetPhysicalDeviceMemoryProperties(inDevice, &mMemoryProperties);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);
    mQueueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, mQueueFamilyProperties.data());

    auto graphicsIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto& prop) {
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT);
    });
    if (graphicsIt != mQueueFamilyProperties.end())
        mGraphicsFamily = static_cast<uint32_t>(std::distance(mQueueFamilyProperties.begin(), graphicsIt));

    auto computeIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto& prop) {
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT);
    });
    if (computeIt != mQueueFamilyProperties.end())
        mComputeFamily = static_cast<uint32_t>(std::distance(mQueueFamilyProperties.begin(), computeIt));
}

DeviceVK::~DeviceVK()
{
    if (mDevice && mGraphicsCommandPool)
        vkDestroyCommandPool(mDevice, mGraphicsCommandPool, nullptr);
    if (mDevice)
        vkDestroyDevice(mDevice, nullptr);
}

DeviceType DeviceVK::type() const
{
    DeviceType type = DeviceType::Unknown;
    switch (mPhysicalDeviceProperties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
        type = DeviceType::DiscreteGPU;
        break;
    }
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
        type = DeviceType::IntegratedGPU;
        break;
    }
    default: {
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

bool DeviceVK::initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces)
{
    static std::unordered_map<uint32_t, std::string> vendorNames = {
        { 0x1002, "AMD" },
        { 0x1010, "ImgTec" },
        { 0x10DE, "NVIDIA" },
        { 0x13B5, "ARM" },
        { 0x5143, "Qualcomm" },
        { 0x8086, "INTEL" }
    };
    SIGMA_INFO("Graphics API: Vulkan");
    SIGMA_INFO("Vender: {}", vendorNames.count(mPhysicalDeviceProperties.vendorID) ? vendorNames[mPhysicalDeviceProperties.vendorID] : "Unknown");
    SIGMA_INFO("Model: {}", mPhysicalDeviceProperties.deviceName);
    SIGMA_INFO("API Version: {}.{}.{}", (mPhysicalDeviceProperties.apiVersion >> 22) & 0x3FF, (mPhysicalDeviceProperties.apiVersion >> 12) & 0x3FF, mPhysicalDeviceProperties.apiVersion & 0xFFF);
    SIGMA_INFO("Driver Version: {}.{}.{}", (mPhysicalDeviceProperties.driverVersion >> 22) & 0x3FF, (mPhysicalDeviceProperties.driverVersion >> 12) & 0x3FF, mPhysicalDeviceProperties.driverVersion & 0xFFF);

    // TODO add a way to specifiy required device extensions
    if (inSurfaces.size())
        mRequiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    // Check if required extensions exists
    std::vector<VkExtensionProperties> extensionProperties;
    uint32_t extensionCount;
    if (vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate extensions!");
        return false;
    }
    extensionProperties.resize(extensionCount);
    if (vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &extensionCount, extensionProperties.data()) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate extensions!");
        return false;
    }

    // Find and report about missing extensions
    auto extIt = std::remove_if(mRequiredExtensions.begin(), mRequiredExtensions.end(), [&](auto ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(extensionProperties.begin(), extensionProperties.end(), [&](const auto& prop) {
            return prop.extensionName == strName;
        });
        return it == extensionProperties.end();
    });

    for (auto it = extIt; it != mRequiredExtensions.end(); it++) {
        SIGMA_ERROR("Missing Required Vulkan Extension: {}", *it);
    }

    if (extIt != mRequiredExtensions.end()) {
        SIGMA_INFO("Supported Vulkan Extensions: {}", fmt::join(extensionProperties.begin(), extensionProperties.end(), ","));
        return false;
    }

    // Check if layers exists
    std::vector<VkLayerProperties> layerProperties;
    uint32_t layerCount;
    if (vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate instance layers!");
        return false;
    }
    layerProperties.resize(layerCount);
    if (vkEnumerateDeviceLayerProperties(mPhysicalDevice, &layerCount, layerProperties.data()) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate instance layers!");
        return false;
    }

    // Find and report missing layers
    auto layerIt = std::remove_if(mEnabledLayers.begin(), mEnabledLayers.end(), [&](const auto& ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(layerProperties.begin(), layerProperties.end(), [&](const auto& prop) {
            return prop.layerName == strName;
        });
        return it == layerProperties.end();
    });

    for (auto it = layerIt; it != mEnabledLayers.end(); it++) {
        SIGMA_WARN("Skipping Missing Vulkan Layer: {}", *it);
    }

    if (layerIt != mEnabledLayers.end()) {
        SIGMA_INFO("Supported Vulkan Layers: {}", fmt::join(layerProperties.begin(), layerProperties.end(), ","));
        mEnabledLayers.erase(layerIt, mEnabledLayers.end());
    }

    // TODO add a way to specifiy required queues
    std::set<uint32_t> queueFamilies;

    std::vector<SurfaceSwapChainInfoVK> surfaceSwapChainInfos;
    for (auto surface : inSurfaces) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<SurfaceVK>(surface), "Incorrect surface type!");
        auto vksurface = std::static_pointer_cast<SurfaceVK>(surface);
        auto swapChainInfo = getSwapChainInfo(vksurface);
        if (!swapChainInfo.has_value() || !swapChainInfo->presentFamily.has_value()) {
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
    for (auto family : queueFamilies) {
        queueCreateInfos.push_back({});
        queueCreateInfos.back().sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos.back().queueCount = 1;
        queueCreateInfos.back().pQueuePriorities = &queuePriority;
        queueCreateInfos.back().queueFamilyIndex = family;
    }

    // TODO fill out device features
    VkPhysicalDeviceFeatures deviceFeatures = {};

    std::vector<const char*> enabledLayers(mEnabledLayers.size());
    std::transform(mEnabledLayers.begin(), mEnabledLayers.end(), enabledLayers.begin(), [](const auto& ext) {
        return ext.c_str();
    });
    std::vector<const char*> enabledExtensions(mRequiredExtensions.size());
    std::transform(mRequiredExtensions.begin(), mRequiredExtensions.end(), enabledExtensions.begin(), [](const auto& ext) {
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

    if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create Vulkan logical device!");
        return false;
    }

    if (mGraphicsFamily.has_value()) {
        VkCommandPoolCreateInfo graphicsPoolInfo = {};
        graphicsPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        graphicsPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        graphicsPoolInfo.queueFamilyIndex = mGraphicsFamily.value();

        if (vkCreateCommandPool(mDevice, &graphicsPoolInfo, nullptr, &mGraphicsCommandPool) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create graphics command pool!");
            return false;
        }

        vkGetDeviceQueue(mDevice, mGraphicsFamily.value(), 0, &mGraphicsQueue);
    }

    for (std::size_t i = 0; i < inSurfaces.size(); ++i) {
        auto surface = std::static_pointer_cast<SurfaceVK>(inSurfaces[i]);
        if (!surface->createSwapChain(shared_from_this(), surfaceSwapChainInfos[i])) {
            SIGMA_ERROR("Could not create surface swapchain!");
            return false;
        }
    }

    return true;
}

std::shared_ptr<CommandBuffer> DeviceVK::createCommandBuffer()
{
    if (mGraphicsCommandPool == nullptr)
        return nullptr;

    auto commandBuffer = std::make_shared<CommandBufferVK>(shared_from_this(), mGraphicsCommandPool);
    if (!commandBuffer->initialize()) {
        return nullptr;
    }
    return commandBuffer;
}

std::shared_ptr<Shader> DeviceVK::createShader(ShaderType inType, const std::string& inSourcePath)
{
    std::ifstream file("vulkan/" + inSourcePath + ".glsl", std::ios::ate | std::ios::binary);
    if (!file.is_open())
        return nullptr;
    size_t fsize = file.tellg();
    std::vector<uint32_t> data(fsize / sizeof(uint32_t));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(data.data()), fsize);

    std::shared_ptr<ShaderVK> shader = std::make_shared<ShaderVK>(inType, shared_from_this());
    if (!shader->initialize(data)) {
        return nullptr;
    }
    return shader;
}

std::shared_ptr<RenderPass> DeviceVK::createRenderPass(const RenderPassCreateParams& inParams)
{
    auto renderPass = std::make_shared<RenderPassVK>(shared_from_this());
    if (!renderPass->initialize(inParams))
        return nullptr;
    return renderPass;
}

std::shared_ptr<Pipeline> DeviceVK::createPipeline(const PipelineCreateParams& inParams)
{
    std::shared_ptr<PipelineVK> pipeline = std::make_shared<PipelineVK>(shared_from_this());
    if (!pipeline->initialize(inParams)) {
        return nullptr;
    }
    return pipeline;
}

std::shared_ptr<VertexBuffer> DeviceVK::createVertexBuffer(const VertexLayout& inLayout, uint64_t inSize)
{
    auto vertexBuffer = std::make_shared<VertexBufferVK>(shared_from_this(), inLayout);
    if (!vertexBuffer->initialize(inSize)) {
        return nullptr;
    }
    return vertexBuffer;
}

std::shared_ptr<IndexBuffer> DeviceVK::createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize)
{
    auto indexBuffer = std::make_shared<IndexBufferVK>(shared_from_this(), inPrimitive, inDataType);
    if (!indexBuffer->initialize(inSize)) {
        return nullptr;
    }
    return indexBuffer;
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

VkResult DeviceVK::createBuffer(VkBufferCreateInfo* inBufferCreateInfo, VkMemoryPropertyFlagBits inProperties, VkBuffer* outBuffer, VkDeviceMemory* outMemory)
{
    VkMemoryRequirements requirements;
    VkMemoryAllocateInfo allocInfo = {};

    VkResult result = vkCreateBuffer(mDevice, inBufferCreateInfo, nullptr, outBuffer);
    if (result != VK_SUCCESS)
        goto done;

    vkGetBufferMemoryRequirements(mDevice, *outBuffer, &requirements);

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits, inProperties);

    result = vkAllocateMemory(mDevice, &allocInfo, nullptr, outMemory);
    if (result != VK_SUCCESS)
        goto done;

    result = vkBindBufferMemory(mDevice, *outBuffer, *outMemory, 0);
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

VkResult DeviceVK::copyBuffer(VkBuffer inDstBuffer, VkBuffer inSrcBuffer, uint64_t inSize)
{
    VkResult result;
    VkCommandBufferAllocateInfo allocInfo = {};
    VkCommandBufferBeginInfo beginInfo = {};
    VkSubmitInfo submitInfo = {};
    VkCommandBuffer commandBuffer;
    VkBufferCopy copyRegion = { 0, 0, inSize };

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = mGraphicsCommandPool;
    allocInfo.commandBufferCount = 1;

    result = vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);
    if (result != VK_SUCCESS)
        goto done;

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
    if (result != VK_SUCCESS)
        goto done;

    vkCmdCopyBuffer(commandBuffer, inSrcBuffer, inDstBuffer, 1, &copyRegion);

    result = vkEndCommandBuffer(commandBuffer);
    if (result != VK_SUCCESS)
        goto done;

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    result = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, nullptr);
    if (result != VK_SUCCESS)
        goto done;

    result = vkQueueWaitIdle(mGraphicsQueue);

    assert(result == VK_SUCCESS);
done:
    if (commandBuffer)
        vkFreeCommandBuffers(mDevice, mGraphicsCommandPool, 1, &commandBuffer);

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

    for (uint32_t i = 0; i < mQueueFamilyProperties.size(); ++i) {
        VkBool32 support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, i, inSurface->handle(), &support);
        if (support == VK_TRUE) {
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
    for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; ++i) {
        if ((inTypeFilter & (1 << i)) && ((mMemoryProperties.memoryTypes[i].propertyFlags & inProperties) == inProperties)) {
            return i;
        }
    }

    return std::numeric_limits<uint32_t>::max();
}