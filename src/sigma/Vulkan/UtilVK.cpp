#include <sigma/Vulkan/UtilVK.hpp>

#include <vulkan/vulkan.h>

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
