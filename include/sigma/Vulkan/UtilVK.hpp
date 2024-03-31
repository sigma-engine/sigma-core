#pragma once

#include <sigma/DataTypes.hpp>
#include <sigma/Log.hpp>

#include <vulkan/vulkan.h>

#include <ostream>
#include <string>

// clang-format off
#ifdef NDEBUG
#define CHECK_VK(expression) {(expression);}
#else
#define CHECK_VK(expression) {auto CHECK_VK_RESULT = (expression); SIGMA_ASSERT(CHECK_VK_RESULT == VK_SUCCESS, "Vulkan Error: {} (0x{:x})", convertErrorStringVK(CHECK_VK_RESULT), CHECK_VK_RESULT);}
#endif
// clang-format on

struct VkExtensionProperties;
struct VkLayerProperties;

enum class ImageFormat;
enum class ShaderType;
enum class DescriptorType;
enum class SamplerWarpMode;
enum class SamplerFilterMode;
enum class SamplerMipmapMode;

namespace fmt
{
template <>
struct formatter<VkExtensionProperties> : formatter<string_view>
{
	template <typename FormatContext>
	auto format(VkExtensionProperties prop, FormatContext& ctx) const
	{
		string_view name = prop.extensionName;
		return formatter<string_view>::format(name, ctx);
	}
};

template <>
struct formatter<VkLayerProperties> : formatter<string_view>
{
	template <typename FormatContext>
	auto format(VkLayerProperties prop, FormatContext& ctx) const
	{
		string_view name = prop.layerName;
		return formatter<string_view>::format(name, ctx);
	}
};

template <>
struct formatter<VkResult> : formatter<string_view>
{
	template <typename FormatContext>
	auto format(VkResult hr, FormatContext& ctx) const
	{
		switch(hr)
		{
		case VK_SUCCESS:
			return formatter<string_view>::format("VK_SUCCESS", ctx);
		case VK_NOT_READY:
			return formatter<string_view>::format("VK_NOT_READY", ctx);
		case VK_TIMEOUT:
			return formatter<string_view>::format("VK_TIMEOUT", ctx);
		case VK_EVENT_SET:
			return formatter<string_view>::format("VK_EVENT_SET", ctx);
		case VK_EVENT_RESET:
			return formatter<string_view>::format("VK_EVENT_RESET", ctx);
		case VK_INCOMPLETE:
			return formatter<string_view>::format("VK_INCOMPLETE", ctx);
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			return formatter<string_view>::format("VK_ERROR_OUT_OF_HOST_MEMORY", ctx);
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			return formatter<string_view>::format("VK_ERROR_OUT_OF_DEVICE_MEMORY", ctx);
		case VK_ERROR_INITIALIZATION_FAILED:
			return formatter<string_view>::format("VK_ERROR_INITIALIZATION_FAILED", ctx);
		case VK_ERROR_DEVICE_LOST:
			return formatter<string_view>::format("VK_ERROR_DEVICE_LOST", ctx);
		case VK_ERROR_MEMORY_MAP_FAILED:
			return formatter<string_view>::format("VK_ERROR_MEMORY_MAP_FAILED", ctx);
		case VK_ERROR_LAYER_NOT_PRESENT:
			return formatter<string_view>::format("VK_ERROR_LAYER_NOT_PRESENT", ctx);
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			return formatter<string_view>::format("VK_ERROR_EXTENSION_NOT_PRESENT", ctx);
		case VK_ERROR_FEATURE_NOT_PRESENT:
			return formatter<string_view>::format("VK_ERROR_FEATURE_NOT_PRESENT", ctx);
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			return formatter<string_view>::format("VK_ERROR_INCOMPATIBLE_DRIVER", ctx);
		case VK_ERROR_TOO_MANY_OBJECTS:
			return formatter<string_view>::format("VK_ERROR_TOO_MANY_OBJECTS", ctx);
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			return formatter<string_view>::format("VK_ERROR_FORMAT_NOT_SUPPORTED", ctx);
		case VK_ERROR_FRAGMENTED_POOL:
			return formatter<string_view>::format("VK_ERROR_FRAGMENTED_POOL", ctx);
		case VK_ERROR_UNKNOWN:
			return formatter<string_view>::format("VK_ERROR_UNKNOWN", ctx);
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			return formatter<string_view>::format("VK_ERROR_OUT_OF_POOL_MEMORY", ctx);
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:
			return formatter<string_view>::format("VK_ERROR_INVALID_EXTERNAL_HANDLE", ctx);
		case VK_ERROR_FRAGMENTATION:
			return formatter<string_view>::format("VK_ERROR_FRAGMENTATION", ctx);
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
			return formatter<string_view>::format("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS", ctx);
		case VK_PIPELINE_COMPILE_REQUIRED:
			return formatter<string_view>::format("VK_PIPELINE_COMPILE_REQUIRED", ctx);
		case VK_ERROR_SURFACE_LOST_KHR:
			return formatter<string_view>::format("VK_ERROR_SURFACE_LOST_KHR", ctx);
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			return formatter<string_view>::format("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR", ctx);
		case VK_SUBOPTIMAL_KHR:
			return formatter<string_view>::format("VK_SUBOPTIMAL_KHR", ctx);
		case VK_ERROR_OUT_OF_DATE_KHR:
			return formatter<string_view>::format("VK_ERROR_OUT_OF_DATE_KHR", ctx);
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			return formatter<string_view>::format("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR", ctx);
		case VK_ERROR_VALIDATION_FAILED_EXT:
			return formatter<string_view>::format("VK_ERROR_VALIDATION_FAILED_EXT", ctx);
		case VK_ERROR_INVALID_SHADER_NV:
			return formatter<string_view>::format("VK_ERROR_INVALID_SHADER_NV", ctx);
		case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
			return formatter<string_view>::format("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR", ctx);
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
			return formatter<string_view>::format("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT", ctx);
		case VK_ERROR_NOT_PERMITTED_KHR:
			return formatter<string_view>::format("VK_ERROR_NOT_PERMITTED_KHR", ctx);
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			return formatter<string_view>::format("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT", ctx);
		case VK_THREAD_IDLE_KHR:
			return formatter<string_view>::format("VK_THREAD_IDLE_KHR", ctx);
		case VK_THREAD_DONE_KHR:
			return formatter<string_view>::format("VK_THREAD_DONE_KHR", ctx);
		case VK_OPERATION_DEFERRED_KHR:
			return formatter<string_view>::format("VK_OPERATION_DEFERRED_KHR", ctx);
		case VK_OPERATION_NOT_DEFERRED_KHR:
			return formatter<string_view>::format("VK_OPERATION_NOT_DEFERRED_KHR", ctx);
		case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
			return formatter<string_view>::format("VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR", ctx);
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
			return formatter<string_view>::format("VK_ERROR_COMPRESSION_EXHAUSTED_EXT", ctx);
		case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
			return formatter<string_view>::format("VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT", ctx);
		case VK_RESULT_MAX_ENUM:
			return formatter<string_view>::format("VK_RESULT_MAX_ENUM", ctx);
		default:
			return formatter<string_view>::format("UNKNOWN", ctx);
		}
	}
};
}

VkFormat convertImageFormatVK(ImageFormat inFormat);

ImageFormat convertImageFormatVK(VkFormat inFormat);

VkShaderStageFlagBits convertShaderTypeVK(ShaderType inType);

VkFormat formatForDataTypeVK(DataType inType);

VkDescriptorType convertDescriptorTypeVK(DescriptorType inType);

std::string convertErrorStringVK(VkResult inResult);

VkSamplerAddressMode convertSamplerWarpModeVK(SamplerWarpMode inMode);

VkFilter convertSamplerFilterModeVK(SamplerFilterMode inMode);

VkSamplerMipmapMode convertSamplerMipmapModeVK(SamplerMipmapMode inMode);