#include <sigma/Vulkan/DescriptorSetVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
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

    std::vector<VkDescriptorBufferInfo> bufferInfos;
    std::vector<VkWriteDescriptorSet> descriptorWrites;
    bufferInfos.resize(inParams.uniformBuffers.size());
    descriptorWrites.resize(inParams.uniformBuffers.size());
    mUniformBuffers.resize(inParams.uniformBuffers.size());

    // TODO: This code needs to be generalized and checked!
    for (uint32_t i = 0; i < inParams.uniformBuffers.size(); ++i) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<UniformBufferVK>(inParams.uniformBuffers[i]), "Must use vulkan uniform buffer with vulkan descriptor set!");
        auto buffer = std::static_pointer_cast<UniformBufferVK>(inParams.uniformBuffers[i]);
        mUniformBuffers[i] = buffer;
        bufferInfos[i].buffer = buffer->handle();
        bufferInfos[i].offset = 0;
        bufferInfos[i].range = buffer->size();

        descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[i].dstSet = mHandle;
        descriptorWrites[i].dstBinding = i;
        descriptorWrites[i].dstArrayElement = 0;
        descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[i].descriptorCount = 1;
        descriptorWrites[i].pBufferInfo = &bufferInfos[i];
        descriptorWrites[i].pImageInfo = nullptr;
        descriptorWrites[i].pTexelBufferView = nullptr;
    }

    vkUpdateDescriptorSets(mDevice->handle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    return true;
}