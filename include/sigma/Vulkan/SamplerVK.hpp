#pragma once

#include <sigma/Sampler.hpp>

#include <vulkan/vulkan.hpp>

#include <memory>

class DeviceVK;

class Sampler2DVK : public Sampler2D {
public:
    Sampler2DVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~Sampler2DVK();

    VkSampler handle() const { return mHandle; }

    bool initialize(const SamplerCreateParams &inParams);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;

    VkSampler mHandle = nullptr;
};