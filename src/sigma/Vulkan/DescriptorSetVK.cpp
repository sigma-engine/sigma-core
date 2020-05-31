#include <sigma/Vulkan/DescriptorSetVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/SamplerVK.hpp>
#include <sigma/Vulkan/TextureVK.hpp>
#include <sigma/Vulkan/UniformBufferVK.hpp>
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

bool DescriptorSetLayoutVK::initialize(const std::vector<DescriptorSetLayoutBinding>& inBindings)
{
	VkResult result;
	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bindings.resize(inBindings.size());
	for (size_t i = 0; i < inBindings.size(); ++i) {
		bindings[i].binding = inBindings[i].binding;
		bindings[i].descriptorType = convertDescriptorTypeVK(inBindings[i].type);
		bindings[i].descriptorCount = inBindings[i].count;
		bindings[i].pImmutableSamplers = nullptr;
		bindings[i].stageFlags = VK_SHADER_STAGE_ALL; // TODO: This is crap
	}

	VkDescriptorSetLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	createInfo.pBindings = bindings.data();

	CHECK_VK(result = vkCreateDescriptorSetLayout(mDevice->handle(), &createInfo, nullptr, &mHandle));
	return result == VK_SUCCESS;
}

DescriptorSetVK::DescriptorSetVK(std::shared_ptr<DeviceVK> inDevice, VkDescriptorPool inDescriptorPool)
	: mDevice(inDevice)
	, mDescriptorPool(inDescriptorPool)
{
}

DescriptorSetVK::~DescriptorSetVK()
{
	if (mDevice && mHandle)
		CHECK_VK(vkFreeDescriptorSets(mDevice->handle(), mDescriptorPool, 1, &mHandle));
}

std::shared_ptr<DescriptorSetLayout> DescriptorSetVK::layout() const
{
	return mLayout;
}

bool DescriptorSetVK::initialize(const DescriptorSetCreateParams& inParams)
{
	VkResult result;
	SIGMA_ASSERT(std::dynamic_pointer_cast<DescriptorSetLayoutVK>(inParams.layout), "Must use vulkan descriptor set layout with vulkan descriptor set!");
	mLayout = std::static_pointer_cast<DescriptorSetLayoutVK>(inParams.layout);
	VkDescriptorSetLayout layout = mLayout->handle();
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = mDescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	CHECK_VK(result = vkAllocateDescriptorSets(mDevice->handle(), &allocInfo, &mHandle));
	if (result != VK_SUCCESS)
		return false;

	uint32_t wi = 0;
	uint32_t i = 0;
	std::vector<VkWriteDescriptorSet> descriptorWrites;
	descriptorWrites.resize(inParams.uniformBuffers.size() + inParams.imageSamplers.size());

	i = 0;
	std::vector<VkDescriptorBufferInfo> bufferInfos(inParams.uniformBuffers.size());
	for (const auto& [binding, buffer] : inParams.uniformBuffers) {
		SIGMA_ASSERT(std::dynamic_pointer_cast<UniformBufferVK>(buffer), "Must use vulkan uniform buffer with vulkan descriptor set!");
		mUniformBuffers[binding] = std::static_pointer_cast<UniformBufferVK>(buffer);

		bufferInfos[i].buffer = mUniformBuffers[binding]->handle();
		bufferInfos[i].offset = 0; // TODO: Fixme
		bufferInfos[i].range = buffer->size();

		descriptorWrites[wi].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[wi].dstSet = mHandle;
		descriptorWrites[wi].dstBinding = binding;
		descriptorWrites[wi].dstArrayElement = 0;
		descriptorWrites[wi].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[wi].descriptorCount = 1;
		descriptorWrites[wi].pBufferInfo = &bufferInfos[i];
		descriptorWrites[wi].pImageInfo = nullptr;
		descriptorWrites[wi].pTexelBufferView = nullptr;
		i++;
		wi++;
	}

	i = 0;
	std::vector<VkDescriptorImageInfo> imageInfos(inParams.imageSamplers.size());
	for (const auto& [binding, combindedSampler] : inParams.imageSamplers) {
		SIGMA_ASSERT(std::dynamic_pointer_cast<Texture2DVK>(combindedSampler.image), "Must use vulkan image with vulkan descriptor set!");
		SIGMA_ASSERT(std::dynamic_pointer_cast<Sampler2DVK>(combindedSampler.sampler), "Must use vulkan sampler with vulkan descriptor set!");
		mCombinedImageSamplers[binding].image = std::static_pointer_cast<Texture2DVK>(combindedSampler.image);
		mCombinedImageSamplers[binding].sampler = std::static_pointer_cast<Sampler2DVK>(combindedSampler.sampler);

		imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // TODO : Fixme
		imageInfos[i].imageView = mCombinedImageSamplers[binding].image->imageViewHandle();
		imageInfos[i].sampler = mCombinedImageSamplers[binding].sampler->handle();

		descriptorWrites[wi].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[wi].dstSet = mHandle;
		descriptorWrites[wi].dstBinding = binding;
		descriptorWrites[wi].dstArrayElement = 0;
		descriptorWrites[wi].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[wi].descriptorCount = 1;
		descriptorWrites[wi].pBufferInfo = nullptr;
		descriptorWrites[wi].pImageInfo = &imageInfos[i];
		descriptorWrites[wi].pTexelBufferView = nullptr;

		i++;
		wi++;
	}

	vkUpdateDescriptorSets(mDevice->handle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return true;
}