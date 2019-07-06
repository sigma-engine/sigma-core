#pragma once

#include <cstdint>

enum class DescriptorType {
	UniformBuffer
};

struct DescriptorSetLayoutBinding {
	DescriptorType type;
	uint32_t count;
};

class DescriptorSetLayout {
public:
    virtual ~DescriptorSetLayout() = default;
};

class DescriptorSet {
public:
    virtual ~DescriptorSet() = default;
};