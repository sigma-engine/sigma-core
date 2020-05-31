#pragma once
#include <sigma/DescriptorSet.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

class UniformBufferGL;
class Texture2DGL;
class Sampler2DGL;

class DescriptorSetLayoutGL : public DescriptorSetLayout {
public:
	DescriptorSetLayoutGL(const std::vector<DescriptorSetLayoutBinding>& inBindings);

private:
	std::vector<DescriptorSetLayoutBinding> mBindings;
};

class DescriptorSetGL : public DescriptorSet {
public:
	virtual std::shared_ptr<DescriptorSetLayout> layout() const;

	bool initialize(const DescriptorSetCreateParams& inParams);

	void bind();

public:
	struct CombindedImageSamplerGL {
		std::shared_ptr<Texture2DGL> image;
		std::shared_ptr<Sampler2DGL> sampler;
	};
	std::shared_ptr<DescriptorSetLayoutGL> mLayout = nullptr;
	std::unordered_map<uint32_t, std::shared_ptr<UniformBufferGL>> mUniformBuffers;
	std::unordered_map<uint32_t, CombindedImageSamplerGL> mImageSamplers;
};
