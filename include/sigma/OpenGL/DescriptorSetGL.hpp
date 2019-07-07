#pragma once
#include <sigma/DescriptorSet.hpp>

#include <memory>
#include <vector>

class UniformBufferGL;

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
    std::shared_ptr<DescriptorSetLayoutGL> mLayout = nullptr;
    std::vector<std::shared_ptr<UniformBufferGL>> mUniformBuffers;
};
