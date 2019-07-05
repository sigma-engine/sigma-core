#include <sigma/Vulkan/VertexBufferVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>
#include <sigma/Log.hpp>

VertexBufferVK::VertexBufferVK(std::shared_ptr<DeviceVK> inDevice, const VertexLayout& inLayout)
	: mDevice(inDevice)
	, mLayout(inLayout)
{
}

VertexBufferVK::~VertexBufferVK()
{
	if (mDevice) 
	{
		if (mHandle)
			vkDestroyBuffer(mDevice->handle(), mHandle, nullptr);
		if (mMemory)
			vkFreeMemory(mDevice->handle(), mMemory, nullptr);
	}
}

bool VertexBufferVK::initialize(const VkPhysicalDeviceMemoryProperties &inMemoryProperties, uint64_t inSize)
{
	mSize = inSize;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = mSize;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(mDevice->handle(), &bufferInfo, nullptr, &mHandle) != VK_SUCCESS)
	{
		return false;
	}

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(mDevice->handle(), mHandle, &requirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = requirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(inMemoryProperties, requirements.memoryTypeBits, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

	if (vkAllocateMemory(mDevice->handle(), &allocInfo, nullptr, &mMemory) != VK_SUCCESS)
	{
		return false;
	}

	if (vkBindBufferMemory(mDevice->handle(), mHandle, mMemory, 0) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

const VertexLayout &VertexBufferVK::layout() const
{
	return mLayout;
}

void VertexBufferVK::setData(const void * inData, uint64_t inSize)
{
	void *dstData = nullptr;
	if (vkMapMemory(mDevice->handle(), mMemory, 0, mSize, 0, &dstData) != VK_SUCCESS)
		return;

	memcpy(dstData, inData, std::min(inSize, mSize));
	vkUnmapMemory(mDevice->handle(), mMemory);
}

uint32_t VertexBufferVK::findMemoryType(const VkPhysicalDeviceMemoryProperties &inMemoryProperties, uint32_t inTypeFilter, VkMemoryPropertyFlagBits inProperties) const
{
	for (uint32_t i = 0; i < inMemoryProperties.memoryTypeCount; ++i)
	{
		if ((inTypeFilter  & (1 << i)) && ((inMemoryProperties.memoryTypes[i].propertyFlags & inProperties) == inProperties))
		{
			return i;
		}
	}

	return std::numeric_limits<uint32_t>::max();
}