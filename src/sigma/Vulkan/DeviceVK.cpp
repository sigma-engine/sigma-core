#include <sigma/Vulkan/DeviceVK.hpp>

#include <sigma/Vulkan/SurfaceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

#include <sigma/Log.hpp>


DeviceVK::DeviceVK(VkInstance inInstance, VkPhysicalDevice inDevice, const std::vector<std::string> &inEnabledLayers)
    : mInstance(inInstance)
    , mPhysicalDevice(inDevice)
    , mEnabledLayers(inEnabledLayers)
{
    vkGetPhysicalDeviceProperties(inDevice, &mPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(inDevice, &mPhysicalDeviceFeatures);

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);
    mQueueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, mQueueFamilyProperties.data());

    auto graphicsIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto &prop){
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT);
    });
    if(graphicsIt != mQueueFamilyProperties.end())
        mGraphicsFamily = std::distance(mQueueFamilyProperties.begin(), graphicsIt);

    auto computeIt = std::find_if(mQueueFamilyProperties.begin(), mQueueFamilyProperties.end(), [](const auto &prop){
        return prop.queueCount > 0 && ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT);
    });
    if (computeIt != mQueueFamilyProperties.end())
        mComputeFamily =  std::distance(mQueueFamilyProperties.begin(), computeIt);
}

DeviceVK::~DeviceVK()
{
    if (mDevice) vkDestroyDevice(mDevice, nullptr);
}

DeviceType DeviceVK::type() const
{
    DeviceType type = DeviceType::Unknown;
    switch (mPhysicalDeviceProperties.deviceType) {
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

bool DeviceVK::initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces)
{
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

    if (layerIt != mEnabledLayers.end())
    {
        SIGMA_INFO("Supported Vulkan Layers: {}", fmt::join(layerProperties.begin(), layerProperties.end(), ","));
        mEnabledLayers.erase(layerIt, mEnabledLayers.end());
    }

    // TODO add a way to specifiy required queues
    std::set<uint32_t> queueFamilies;

    std::vector<SurfaceSwapChainInfoVK> surfaceSwapChainInfos;
    for(auto surface: inSurfaces)
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
    for(auto family: queueFamilies)
    {
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

    if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS)
    {
        SIGMA_ERROR("Could not create Vulkan logical device!");
        return false;
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

    return true;
}

std::shared_ptr<Shader> DeviceVK::createShader(ShaderType inType, const std::string& inCode)
{
    SIGMA_ASSERT(false, "Not Implemented!");
    return nullptr;
}

std::shared_ptr<Program> DeviceVK::createProgram(const std::vector<std::shared_ptr<Shader>>& inShaders)
{
    SIGMA_ASSERT(false, "Not Implemented!");
    return nullptr;
}

std::shared_ptr<VertexBuffer> DeviceVK::createVertexBuffer(const std::initializer_list<VertexMemberDescription>& inLayout)
{
    SIGMA_ASSERT(false, "Not Implemented!");
    return nullptr;
}

std::shared_ptr<IndexBuffer> DeviceVK::createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType)
{
    SIGMA_ASSERT(false, "Not Implemented!");
    return nullptr;
}

void DeviceVK::draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer)
{
    SIGMA_ASSERT(false, "Not Implemented!");
}

uint32_t DeviceVK::graphicsQueueFamily() const
{
    return mGraphicsFamily.value();
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

    for(uint32_t i = 0; i < mQueueFamilyProperties.size(); ++i)
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
