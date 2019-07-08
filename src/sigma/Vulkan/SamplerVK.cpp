#include <sigma/Vulkan/SamplerVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

Sampler2DVK::Sampler2DVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

Sampler2DVK::~Sampler2DVK()
{
    if (mDevice && mHandle)
        vkDestroySampler(mDevice->handle(), mHandle, nullptr);
}

bool Sampler2DVK::initialize()
{
    // TODO: Don't hard code this info
    VkResult result;
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 0;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    CHECK_VK(result = vkCreateSampler(mDevice->handle(), &samplerInfo, nullptr, &mHandle));
    return result == VK_SUCCESS;
}