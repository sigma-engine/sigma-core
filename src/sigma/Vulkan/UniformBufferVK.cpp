#include <sigma/Vulkan/UniformBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>

#include <algorithm>

UniformBufferVK::UniformBufferVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

UniformBufferVK::~UniformBufferVK()
{
    if (mDevice) {
        if (mHandle)
            vkDestroyBuffer(mDevice->handle(), mHandle, nullptr);
        if (mMemory)
            vkFreeMemory(mDevice->handle(), mMemory, nullptr);
    }
}

uint64_t UniformBufferVK::size() const
{
	return mSize;
}

void UniformBufferVK::setData(const void* inData, uint64_t inSize)
{
    void* dstData;
    if (vkMapMemory(mDevice->handle(), mMemory, 0, std::min(inSize, mSize), 0, &dstData) != VK_SUCCESS)
        return;

    memcpy(dstData, inData, std::min(inSize, mSize));
    vkUnmapMemory(mDevice->handle(), mMemory);
}

bool UniformBufferVK::initialize(uint64_t inSize)
{
    mSize = inSize;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = mSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    return mDevice->createBuffer(&bufferInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT), &mHandle, &mMemory) == VK_SUCCESS;
}