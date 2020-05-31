#include <sigma/Vulkan/UtilVK.hpp>

#include <sigma/DescriptorSet.hpp>
#include <sigma/Log.hpp>
#include <sigma/RenderPass.hpp>
#include <sigma/Shader.hpp>
#include <sigma/Sampler.hpp>

#include <vulkan/vulkan.h>

VkFormat convertImageFormatVK(ImageFormat inFormat)
{
	switch (inFormat)
	{
	case ImageFormat::UnormB8G8R8A8:
		return VK_FORMAT_B8G8R8A8_UNORM;
	case ImageFormat::UnormR8G8B8A8:
		return VK_FORMAT_R8G8B8A8_UNORM;
	}
	SIGMA_ASSERT(false, "Unknown ImageFormat!");
	return VK_FORMAT_MAX_ENUM;
}

ImageFormat convertImageFormatVK(VkFormat inFormat)
{
	switch (inFormat)
	{
	case VK_FORMAT_B8G8R8A8_UNORM:
		return ImageFormat::UnormB8G8R8A8;
	default:
		SIGMA_ASSERT(false, "Unknown image format!");
	}
	return ImageFormat::UnormB8G8R8A8;
}

VkShaderStageFlagBits convertShaderTypeVK(ShaderType inType)
{
	switch (inType)
	{
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
	switch (inType)
	{
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
	case DescriptorType::ImageSampler:
		return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	default:
		break;
	}
	SIGMA_ASSERT(false, "Unknown DescriptorType");
	return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

std::string convertErrorStringVK(VkResult inResult)
{
	switch (inResult)
	{
	case VK_SUCCESS:
		return "SUCCESS";
	case VK_NOT_READY:
		return "NOT_READY";
	case VK_TIMEOUT:
		return "TIMEOUT";
	case VK_EVENT_SET:
		return "EVENT_SET";
	case VK_EVENT_RESET:
		return "EVENT_RESET";
	case VK_INCOMPLETE:
		return "INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:
		return "ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_FRAGMENTED_POOL:
		return "ERROR_FRAGMENTED_POOL";
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return "ERROR_OUT_OF_POOL_MEMORY";
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return "ERROR_INVALID_EXTERNAL_HANDLE";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "ERROR_SURFACE_LOST_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_SUBOPTIMAL_KHR:
		return "SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "ERROR_VALIDATION_FAILED_EXT";
	case VK_ERROR_INVALID_SHADER_NV:
		return "ERROR_INVALID_SHADER_NV";
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return "ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
	case VK_ERROR_FRAGMENTATION_EXT:
		return "ERROR_FRAGMENTATION_EXT";
	case VK_ERROR_NOT_PERMITTED_EXT:
		return "ERROR_NOT_PERMITTED_EXT";
	case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
		return "ERROR_INVALID_DEVICE_ADDRESS_EXT";
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return "ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
	default:
		break;
	}

	return "ERROR_UNKNOWN";
}

VkSamplerAddressMode convertSamplerWarpModeVK(SamplerWarpMode inMode)
{
	switch (inMode)
	{
	case SamplerWarpMode::ClampToEdge:
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	case SamplerWarpMode::MirroredRepeat:
		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	case SamplerWarpMode::Repeat:
		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
	SIGMA_ASSERT(false, "Unknown SamplerWarpMode");
	return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

VkFilter convertSamplerFilterModeVK(SamplerFilterMode inMode)
{
	switch (inMode)
	{
	case SamplerFilterMode::Nearest:
		return VK_FILTER_NEAREST;
	case SamplerFilterMode::Linear:
		return VK_FILTER_LINEAR;
	case SamplerFilterMode::Cubic:
		return VK_FILTER_CUBIC_IMG;
	}
	SIGMA_ASSERT(false, "Unknown SamplerFilterMode");
	return VK_FILTER_LINEAR;
}

VkSamplerMipmapMode convertSamplerMipmapModeVK(SamplerMipmapMode inMode)
{
	switch (inMode)
	{
	case SamplerMipmapMode::None:
	case SamplerMipmapMode::Nearest:
		return VK_SAMPLER_MIPMAP_MODE_NEAREST;
	case SamplerMipmapMode::Linear:
		return VK_SAMPLER_MIPMAP_MODE_LINEAR;
	}
	SIGMA_ASSERT(false, "Unknown SamplerMipmapMode");
	return VK_SAMPLER_MIPMAP_MODE_NEAREST;
}