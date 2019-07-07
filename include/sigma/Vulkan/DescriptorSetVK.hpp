#pragma once
#include <sigma/DescriptorSet.hpp>

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

class DeviceVK;
class UniformBufferVK;

class DescriptorSetLayoutVK : public DescriptorSetLayout {
public:
    DescriptorSetLayoutVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~DescriptorSetLayoutVK();

    VkDescriptorSetLayout handle() const { return mHandle; }

    bool initialize(const std::vector<DescriptorSetLayoutBinding>& inBindings);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkDescriptorSetLayout mHandle = nullptr;
};

class DescriptorSetVK : public DescriptorSet {
public:
    DescriptorSetVK(std::shared_ptr<DeviceVK> inDevice, VkDescriptorPool inDescriptorPool);

    virtual ~DescriptorSetVK();

	virtual std::shared_ptr<DescriptorSetLayout> layout() const override;

    VkDescriptorSet handle() const { return mHandle; }

    bool initialize(const DescriptorSetCreateParams &inParams);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    std::shared_ptr<DescriptorSetLayoutVK> mLayout = nullptr;
    VkDescriptorPool mDescriptorPool = nullptr;
    VkDescriptorSet mHandle = nullptr;

	std::vector<std::shared_ptr<UniformBufferVK>> mUniformBuffers;
};