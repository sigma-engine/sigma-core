#include <sigma/Vulkan/UtilVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/RenderPass.hpp>
#include <sigma/Shader.hpp>
#include <sigma/DescriptorSet.hpp>

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

VkFormat formatForDataTypeVK(DataType inType)
{
    switch (inType) {
    case DataType::Float:
        return VK_FORMAT_R32_SFLOAT;
    case DataType::Vec2:
        return VK_FORMAT_R32G32_SFLOAT;
    case DataType::Vec3:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case DataType::Vec4:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case DataType::UInt:
        return VK_FORMAT_R32_UINT;
    case DataType::UShort:
        return VK_FORMAT_R16_UINT;
    }
    SIGMA_ASSERT(false, "Unknown DataType");
    return VK_FORMAT_UNDEFINED;
}

VkDescriptorType convertDescriptorTypeVK(DescriptorType inType)
{
	switch (inType)
	{
	case DescriptorType::UniformBuffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	default:
		break;
	}
	SIGMA_ASSERT(false, "Unknown DescriptorType");
	return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}