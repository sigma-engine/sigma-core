#include <sigma/Vulkan/UniformBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

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

		if (mAllocation) {
			if (mData)
				vmaUnmapMemory(mDevice->allocator(), mAllocation);

			vmaFreeMemory(mDevice->allocator(), mAllocation);
		}
	}
}

uint64_t UniformBufferVK::size() const
{
	return mSize;
}

void UniformBufferVK::setData(const void* inData, uint64_t inSize)
{
	memcpy(mData, inData, std::min(inSize, mSize));
}

bool UniformBufferVK::initialize(uint64_t inSize)
{
	VkResult result;
	mSize = inSize;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = mSize;
	bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

#if 0
	CHECK_VK(result = mDevice->createBuffer(&bufferInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT), &mHandle, &mMemory));
#else
	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	CHECK_VK(result = vmaCreateBuffer(mDevice->allocator(), &bufferInfo, &allocInfo, &mHandle, &mAllocation, nullptr));
	if (result != VK_SUCCESS)
		return false;

	CHECK_VK(result = vmaMapMemory(mDevice->allocator(), mAllocation, &mData));
#endif
	return result == VK_SUCCESS;
}