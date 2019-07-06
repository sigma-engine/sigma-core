#pragma once
#include <sigma/DescriptorSet.hpp>

#include <memory>
#include <vector>

#include <vulkan/vulkan.h>

class DeviceVK;

class DescriptorSetLayoutVK : public DescriptorSetLayout {
public:
    DescriptorSetLayoutVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~DescriptorSetLayoutVK();

    VkDescriptorSetLayout handle() const { return mHandle; }

    bool initialize(const std::vector<DescriptorSetLayoutBinding> &inBindings);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkDescriptorSetLayout mHandle = nullptr;
};