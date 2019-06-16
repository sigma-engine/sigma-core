#include <sigma/Vulkan/DeviceVK.hpp>

#include <sigma/Log.hpp>
#include <spdlog/fmt/bundled/ostream.h>

#include <algorithm>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const VkExtensionProperties& prop)
{
    os << prop.extensionName;
    return os;
}

std::ostream& operator<<(std::ostream& os, const VkLayerProperties& prop)
{
    os << prop.layerName;
    return os;
}

#ifndef NDEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT inSeverity,
    VkDebugUtilsMessageTypeFlagsEXT inType,
    const VkDebugUtilsMessengerCallbackDataEXT* inCallbackData,
    void* pUserData)
{
    switch (inSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    {
        SIGMA_TRACE(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    {
        SIGMA_INFO(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    {
        SIGMA_WARN(inCallbackData->pMessage);
        break;
    }
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    {
        SIGMA_ERROR(inCallbackData->pMessage);
        break;
    }
    default:
    {
        break;
    }
    }

    return VK_FALSE;
}

#endif

DeviceVK::~DeviceVK()
{
#ifndef NDEBUG
    if (mDebugMessenger)
        vkDestroyDebugUtilsMessengerEXT(mHandle, mDebugMessenger, nullptr);
#endif
    if (mHandle)
        vkDestroyInstance(mHandle, nullptr);
}

bool DeviceVK::initialize(const std::set<std::string> &s)
{
    std::set<std::string> requiredExtensions(s.begin(), s.end());
#ifndef NDEBUG
    requiredExtensions.insert(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    // Check required extensions exists
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

    std::vector<const char*> enabledExtensions(requiredExtensions.size());
    std::transform(requiredExtensions.begin(), requiredExtensions.end(), enabledExtensions.begin(), [](const auto& ext) {
        return ext.c_str();
    });

    auto extIt = std::remove_if(enabledExtensions.begin(), enabledExtensions.end(), [&](const auto& ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(extensionProperties.begin(), extensionProperties.end(), [&strName](const auto& prop) {
            return prop.extensionName == strName;
        });
        return it == extensionProperties.end();
    });

    for (auto it = extIt; it != enabledExtensions.end(); it++) {
        SIGMA_ERROR("Missing Required Vulkan Extension: {}", *it);
    }

    if (extIt != enabledExtensions.end()) {
        SIGMA_INFO("Supported Vulkan Extensions: {}", fmt::join(extensionProperties.begin(), extensionProperties.end(), ","));
        return false;
    }

    // Check layers exists
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

    std::vector<const char*> enabledLayers = {

        "VK_LAYER_KHRONOS_validation",
        "VK_LAYER_LUNARG_standard_validation"

    };

    auto layerIt = std::remove_if(enabledLayers.begin(), enabledLayers.end(), [&](const auto& ext) {
        auto strName = std::string(ext);
        auto it = std::find_if(layerProperties.begin(), layerProperties.end(), [&strName](const auto& prop) {
            return prop.layerName == strName;
        });
        return it == layerProperties.end();
    });

    for (auto it = layerIt; it != enabledLayers.end(); it++) {
        SIGMA_WARN("Skipping Missing Vulkan Layer: {}", *it);
    }

    if (layerIt != enabledLayers.end())
    {
        SIGMA_INFO("Supported Vulkan Layers: {}", fmt::join(layerProperties.begin(), layerProperties.end(), ","));
        enabledLayers.erase(layerIt, enabledLayers.end());
    }

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

        if (vkCreateDebugUtilsMessengerEXT(mHandle, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS)
        {
            SIGMA_ERROR("Could not create Debug Utils Messenger!");
            return false;
        }
    }
#endif

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
