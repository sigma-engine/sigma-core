#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

class UniformBuffer;
class Texture2D;
class Sampler2D;

enum class DescriptorType {
	UniformBuffer,
	ImageSampler
};

struct DescriptorSetLayoutBinding {
	uint32_t binding;
	DescriptorType type;
	uint32_t count;
};

class DescriptorSetLayout {
public:
	virtual ~DescriptorSetLayout() = default;
};

struct CombinedImageSampler {
	std::shared_ptr<Texture2D> image;
	std::shared_ptr<Sampler2D> sampler;
};

struct DescriptorSetCreateParams {
	std::shared_ptr<DescriptorSetLayout> layout;
	std::unordered_map<uint32_t, std::shared_ptr<UniformBuffer>> uniformBuffers;
	std::unordered_map<uint32_t, CombinedImageSampler> imageSamplers;
};

class DescriptorSet {
public:
	virtual ~DescriptorSet() = default;

	virtual std::shared_ptr<DescriptorSetLayout> layout() const = 0;
};