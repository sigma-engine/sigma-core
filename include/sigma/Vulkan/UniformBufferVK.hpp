#pragma once

#include <sigma/UniformBuffer.hpp>

#include <VulkanMemoryAllocator/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <cstdint>
#include <memory>

class DeviceVK;

class UniformBufferVK : public UniformBuffer {
public:
    UniformBufferVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~UniformBufferVK();

    virtual uint64_t size() const override;

    virtual void setData(const void* inData, uint64_t inSize) override;

    VkBuffer handle() const { return mHandle; }

    bool initialize(uint64_t inSize);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;

    VkBuffer mHandle = nullptr;
    VmaAllocation mAllocation = nullptr;
    void* mData = nullptr;

    uint64_t mSize;
};