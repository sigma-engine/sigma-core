#pragma once

#include <sigma/IndexBuffer.hpp>

#include <VulkanMemoryAllocator/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;

class IndexBufferVK : public IndexBuffer {
public:
    IndexBufferVK(std::shared_ptr<DeviceVK> inDevice, PrimitiveType inPrimitiveType, DataType inDataType);

    virtual ~IndexBufferVK();

    virtual DataType dataType() const override;

    virtual PrimitiveType primitiveType() const override;

    virtual void setData(const void* inData, uint64_t inSize) override;

    VkBuffer handle() const { return mHandle; }

    bool initialize(uint64_t inSize);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkBuffer mHandle = nullptr;
    VmaAllocation mAllocation = nullptr;
    uint64_t mSize = 0;

    PrimitiveType mPrimitiveType;
    DataType mDataType;
};