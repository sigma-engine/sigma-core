#include <sigma/Vulkan/DeviceManagerVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT inSeverity,
    VkDebugUtilsMessageTypeFlagsEXT inType,
    const VkDebugUtilsMessengerCallbackDataEXT* inCallbackData,
    void* pUserData)
{
    switch (inSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
        SIGMA_TRACE(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
        SIGMA_INFO(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
        SIGMA_WARN(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
        SIGMA_ERROR(inCallbackData->pMessage);
        break;
    }
    default: {
        break;
    }
    }

    return VK_FALSE;
}

#endif

DeviceManagerVK::~DeviceManagerVK()
{
    mDevices.clear();

#ifndef NDEBUG
    if (mDebugMessenger)
        vkDestroyDebugUtilsMessengerEXT(mHandle, mDebugMessenger, nullptr);
#endif
    if (mHandle)
        vkDestroyInstance(mHandle, nullptr);
}

bool DeviceManagerVK::initialize(const std::set<std::string>& s)
{
	uint32_t vulkanVersion;
	if (vkEnumerateInstanceVersion(&vulkanVersion) != VK_SUCCESS)
	{
		SIGMA_ERROR("Could not enumerate vulkan instance version!");
		return false;
	}

	if (vulkanVersion < VK_MAKE_VERSION(1, 1, 0))
	{
		SIGMA_ERROR("Vulkan 1.1.0 required!");
		return false;
	}

    mRequiredExtensions.insert(mRequiredExtensions.end(), s.begin(), s.end());

#ifndef NDEBUG
    mRequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    mEnabledLayers.push_back("VK_LAYER_KHRONOS_validation");
    mEnabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

    // Check if required extensions exists
    std::vector<VkExtensionProperties> extensionProperties;
    uint32_t extensionCount;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate extensions!");
        return false;
    }
    extensionProperties.resize(extensionCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data()) != VK_SUCCESS) {
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
    if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not enumerate instance layers!");
        return false;
    }
    layerProperties.resize(layerCount);
    if (vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data()) != VK_SUCCESS) {
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

    // Create the vectors for vulkan create info
    std::vector<const char*> enabledLayers(mEnabledLayers.size());
    std::transform(mEnabledLayers.begin(), mEnabledLayers.end(), enabledLayers.begin(), [](const auto& ext) {
        return ext.c_str();
    });
    std::vector<const char*> enabledExtensions(mRequiredExtensions.size());
    std::transform(mRequiredExtensions.begin(), mRequiredExtensions.end(), enabledExtensions.begin(), [](const auto& ext) {
        return ext.c_str();
    });

    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        appInfo.pApplicationName = "<TOOD>";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "sigma";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.ppEnabledExtensionNames = enabledExtensions.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
        createInfo.ppEnabledLayerNames = enabledLayers.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());

        if (vkCreateInstance(&createInfo, nullptr, &mHandle) != VK_SUCCESS)
            return false;
    }

#ifndef NDEBUG
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = vkDebugCallback;
        createInfo.pUserData = nullptr;

        if (vkCreateDebugUtilsMessengerEXT(mHandle, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS) {
            SIGMA_ERROR("Could not create Debug Utils Messenger!");
            return false;
        }
    }
#endif

    std::vector<VkPhysicalDevice> physicalDevices;
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(mHandle, &physicalDeviceCount, nullptr) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create enumerate physical devices!");
        return false;
    }
    physicalDevices.resize(physicalDeviceCount);
    if (vkEnumeratePhysicalDevices(mHandle, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
        SIGMA_ERROR("Could not create enumerate physical devices!");
        return false;
    }
    mDevices.resize(physicalDevices.size());

    std::transform(physicalDevices.begin(), physicalDevices.end(), mDevices.begin(), [&](const auto& device) {
        return std::make_shared<DeviceVK>(mHandle, device, mEnabledLayers);
    });

    return true;
}

void DeviceManagerVK::enumerateSurfaceDevices(std::shared_ptr<Surface> inSurface, std::vector<std::shared_ptr<Device>>& outDevices)
{
    outDevices.clear();
    for (auto device : mDevices) {
        if (device->supportsSurface(inSurface)) {
            outDevices.push_back(device);
        }
    }

    sortDevices(outDevices);
}
