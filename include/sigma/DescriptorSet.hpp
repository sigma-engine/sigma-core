#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class UniformBuffer;

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

struct DescriptorSetCreateParams
{
	std::shared_ptr<DescriptorSetLayout> layout;
	std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers;
};

class DescriptorSet {
public:
    virtual ~DescriptorSet() = default;

	virtual std::shared_ptr<DescriptorSetLayout> layout() const = 0;
};