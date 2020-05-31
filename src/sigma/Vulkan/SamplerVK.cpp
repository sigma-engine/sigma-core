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

bool Sampler2DVK::initialize(const SamplerCreateParams &inParams)
{
	// TODO: Don't hard code this info
	VkResult result;
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.addressModeU = convertSamplerWarpModeVK(inParams.warpU);
	samplerInfo.addressModeV = convertSamplerWarpModeVK(inParams.warpV);
	samplerInfo.addressModeW = convertSamplerWarpModeVK(inParams.warpW);
	samplerInfo.anisotropyEnable = VK_FALSE;
	samplerInfo.maxAnisotropy = 0;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.magFilter = convertSamplerFilterModeVK(inParams.magFilter);
	samplerInfo.minFilter = convertSamplerFilterModeVK(inParams.minFilter);
	samplerInfo.mipmapMode = convertSamplerMipmapModeVK(inParams.mipmapMode);
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	CHECK_VK(result = vkCreateSampler(mDevice->handle(), &samplerInfo, nullptr, &mHandle));
	return result == VK_SUCCESS;
}