#include <sigma/Vulkan/DescriptorSetVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

DescriptorSetLayoutVK::DescriptorSetLayoutVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

DescriptorSetLayoutVK::~DescriptorSetLayoutVK()
{
    if (mDevice && mHandle)
        vkDestroyDescriptorSetLayout(mDevice->handle(), mHandle, nullptr);
}

bool DescriptorSetLayoutVK::initialize(const std::vector<DescriptorSetLayoutBinding> &inBindings)
{
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bindings.resize(inBindings.size());
	for (size_t i = 0; i < inBindings.size(); ++i)
	{
		bindings[i].binding = static_cast<uint32_t>(i);
		bindings[i].descriptorType = convertDescriptorTypeVK(inBindings[i].type);
		bindings[i].descriptorCount = inBindings[i].count;
		bindings[i].pImmutableSamplers = nullptr;
		bindings[i].stageFlags = VK_SHADER_STAGE_ALL; // TODO: This is crap
	}

    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	createInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(mDevice->handle(), &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		return false;
	
    return true;
}