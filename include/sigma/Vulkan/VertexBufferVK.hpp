#pragma once

#include <sigma/VertexBuffer.hpp>
#include <sigma/VertexLayout.hpp>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;

class VertexBufferVK : public VertexBuffer {
public:
    VertexBufferVK(std::shared_ptr<DeviceVK> inDevice, const VertexLayout& inLayout);

    virtual ~VertexBufferVK();

    virtual const VertexLayout& layout() const override;

    virtual void setData(const void* inData, uint64_t inSize) override;

    VkBuffer handle() const { return mHandle; }

    bool initialize(uint64_t inSize);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VertexLayout mLayout;
    VkBuffer mHandle = nullptr;
    VmaAllocation mAllocation = nullptr;
    uint64_t mSize = 0;
};