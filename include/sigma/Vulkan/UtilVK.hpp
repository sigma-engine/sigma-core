#pragma once

#include <sigma/DataTypes.hpp>

#include <spdlog/fmt/bundled/ostream.h>

#include <vulkan/vulkan.h>

#include <ostream>

struct VkExtensionProperties;
struct VkLayerProperties;

enum class ImageFormat;
enum class ShaderType;

std::ostream& operator<<(std::ostream& os, const VkExtensionProperties& prop);

std::ostream& operator<<(std::ostream& os, const VkLayerProperties& prop);

VkFormat convertImageFormatVK(ImageFormat inFormat);

ImageFormat convertImageFormatVK(VkFormat inFormat);

VkShaderStageFlagBits convertShaderTypeVK(ShaderType inType);

VkFormat formatForDataTypeVK(DataType inType);