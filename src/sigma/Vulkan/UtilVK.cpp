#include <sigma/Vulkan/UtilVK.hpp>

#include <sigma/RenderPass.hpp>
#include <sigma/Log.hpp>
#include <sigma/Shader.hpp>

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

VkFormat convertImageFormatVK(ImageFormat inFormat)
{
    switch (inFormat) {
    case ImageFormat::UnormB8G8R8A8:
        return VK_FORMAT_B8G8R8A8_UNORM;
    case ImageFormat::Unknown:
        SIGMA_ASSERT(false, "Unknown ImageFormat!");
    }
    return VK_FORMAT_MAX_ENUM;
}

ImageFormat convertImageFormatVK(VkFormat inFormat)
{
    switch (inFormat) {
    case VK_FORMAT_B8G8R8A8_UNORM:
        return ImageFormat::UnormB8G8R8A8;
    default:
        SIGMA_ASSERT(false, "Unknown image format!");
    }
    return ImageFormat::UnormB8G8R8A8;
}

VkShaderStageFlagBits convertShaderTypeVK(ShaderType inType)
{
    switch (inType) {
    case ShaderType::VertexShader:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderType::FragmentShader:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    SIGMA_ASSERT(false, "Unknown Shader type!");
    return VK_SHADER_STAGE_VERTEX_BIT;
}
