#pragma once
#include <sigma/DescriptorSet.hpp>

#include <memory>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

class DeviceVK;
class UniformBufferVK;
class Texture2DVK;
class Sampler2DVK;

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

    bool initialize(const DescriptorSetCreateParams& inParams);

private:
    struct CombinedImageSamplerVK {
        std::shared_ptr<Texture2DVK> image;
        std::shared_ptr<Sampler2DVK> sampler;
    };

    std::shared_ptr<DeviceVK> mDevice = nullptr;
    std::shared_ptr<DescriptorSetLayoutVK> mLayout = nullptr;
    VkDescriptorPool mDescriptorPool = nullptr;
    VkDescriptorSet mHandle = nullptr;

    std::unordered_map<uint32_t, std::shared_ptr<UniformBufferVK>> mUniformBuffers;
    std::unordered_map<uint32_t, CombinedImageSamplerVK> mCombinedImageSamplers;
};
