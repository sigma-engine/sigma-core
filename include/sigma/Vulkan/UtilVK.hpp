#pragma once

#include <sigma/DataTypes.hpp>
#include <sigma/Log.hpp>

#include <spdlog/fmt/bundled/ostream.h>

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

std::ostream &operator<<(std::ostream &os, const VkExtensionProperties &prop);

std::ostream &operator<<(std::ostream &os, const VkLayerProperties &prop);

VkFormat convertImageFormatVK(ImageFormat inFormat);

ImageFormat convertImageFormatVK(VkFormat inFormat);

VkShaderStageFlagBits convertShaderTypeVK(ShaderType inType);

VkFormat formatForDataTypeVK(DataType inType);

VkDescriptorType convertDescriptorTypeVK(DescriptorType inType);

std::string convertErrorStringVK(VkResult inResult);

VkSamplerAddressMode convertSamplerWarpModeVK(SamplerWarpMode inMode);

VkFilter convertSamplerFilterModeVK(SamplerFilterMode inMode);

VkSamplerMipmapMode convertSamplerMipmapModeVK(SamplerMipmapMode inMode);