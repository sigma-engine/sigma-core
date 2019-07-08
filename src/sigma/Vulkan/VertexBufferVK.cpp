#include <sigma/Vulkan/VertexBufferVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

VertexBufferVK::VertexBufferVK(std::shared_ptr<DeviceVK> inDevice, const VertexLayout& inLayout)
    : mDevice(inDevice)
    , mLayout(inLayout)
{
}

VertexBufferVK::~VertexBufferVK()
{
    if (mDevice) {
        if (mHandle)
            vkDestroyBuffer(mDevice->handle(), mHandle, nullptr);
        if (mMemory)
            vkFreeMemory(mDevice->handle(), mMemory, nullptr);
    }
}

const VertexLayout& VertexBufferVK::layout() const
{
    return mLayout;
}

void VertexBufferVK::setData(const void* inData, uint64_t inSize)
{
    // TODO: This is crap
    VkResult result;
    VkBuffer stagingBuffer = nullptr;
    VkDeviceMemory stagingMemory = nullptr;
    void* dstData = nullptr;
    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = std::min(inSize, mSize);
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    CHECK_VK(result = mDevice->createBuffer(&bufferInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT), &stagingBuffer, &stagingMemory));
    if (result != VK_SUCCESS)
        goto done;

    CHECK_VK(result = vkMapMemory(mDevice->handle(), stagingMemory, 0, bufferInfo.size, 0, &dstData));
    if (result != VK_SUCCESS)
        goto done;

    memcpy(dstData, inData, bufferInfo.size);
    vkUnmapMemory(mDevice->handle(), stagingMemory);

    CHECK_VK(mDevice->copyBufferToBuffer(mHandle, stagingBuffer, bufferInfo.size));

done:
    if (stagingBuffer)
        vkDestroyBuffer(mDevice->handle(), stagingBuffer, nullptr);
    if (stagingMemory)
        vkFreeMemory(mDevice->handle(), stagingMemory, nullptr);
}

bool VertexBufferVK::initialize(uint64_t inSize)
{
    VkResult result;
    mSize = inSize;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = mSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    CHECK_VK(result = mDevice->createBuffer(&bufferInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), &mHandle, &mMemory));
    return result == VK_SUCCESS;
}
