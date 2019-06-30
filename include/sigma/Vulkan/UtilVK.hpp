#pragma once

#include <spdlog/fmt/bundled/ostream.h>

#include <ostream>

struct VkExtensionProperties;
struct VkLayerProperties;

std::ostream& operator<<(std::ostream& os, const VkExtensionProperties& prop);

std::ostream& operator<<(std::ostream& os, const VkLayerProperties& prop);
