#include <sigma/Vulkan/IndexBufferVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

IndexBufferVK::IndexBufferVK(std::shared_ptr<DeviceVK> inDevice, PrimitiveType inPrimitiveType, DataType inDataType)
    : mDevice(inDevice)
    , mPrimitiveType(inPrimitiveType)
    , mDataType(inDataType)
{
}

IndexBufferVK::~IndexBufferVK()
{
    if (mDevice) {
        if (mHandle)
            vkDestroyBuffer(mDevice->handle(), mHandle, nullptr);
        if (mMemory)
            vkFreeMemory(mDevice->handle(), mMemory, nullptr);
    }
}

DataType IndexBufferVK::dataType() const
{
    return mDataType;
}

PrimitiveType IndexBufferVK::primitiveType() const
{
    return mPrimitiveType;
}

void IndexBufferVK::setData(const void* inData, uint64_t inSize)
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

    mDevice->copyBuffer(mHandle, stagingBuffer, bufferInfo.size);

done:
    if (stagingBuffer)
        vkDestroyBuffer(mDevice->handle(), stagingBuffer, nullptr);
    if (stagingMemory)
        vkFreeMemory(mDevice->handle(), stagingMemory, nullptr);
}

bool IndexBufferVK::initialize(uint64_t inSize)
{
    mSize = inSize;
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = mSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult result;
	CHECK_VK(result = mDevice->createBuffer(&bufferInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), &mHandle, &mMemory));

	return result == VK_SUCCESS;
}