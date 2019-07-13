#include <sigma/Vulkan/TextureVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

Texture2DVK::Texture2DVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

Texture2DVK::~Texture2DVK()
{
    if (mDevice) {
        if (mImageView)
            vkDestroyImageView(mDevice->handle(), mImageView, nullptr);
        if (mImage)
            vkDestroyImage(mDevice->handle(), mImage, nullptr);
        if (mMemory)
            vkFreeMemory(mDevice->handle(), mMemory, nullptr);
    }
}

bool Texture2DVK::initialize(const TextureCreateParams& inParams)
{
	// TODO texture types other than 2D
	SIGMA_ASSERT(inParams.size.x > 0 && inParams.size.y > 0 && inParams.size.z == 1, "Can only create 2d textures for now!");

	mSize = inParams.size;
    mFormat = inParams.format;

    uint64_t imageSize = mSize.x * mSize.y * imageFormatPixelSize(mFormat);
    VkFormat format = convertImageFormatVK(mFormat);

    VkResult result;
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = mSize.x;
    imageInfo.extent.height = mSize.y;
    imageInfo.extent.depth = mSize.z;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	imageInfo.usage = 0;
	if (inParams.usage.isSet(ImageUsage::Sampler))
		imageInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

	if (inParams.usage.isSet(ImageUsage::ColorAttachment))
		imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (inParams.usage.isSet(ImageUsage::DepthStencilAttachment))
		imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

	if (inParams.pixels)
		imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    CHECK_VK(result = mDevice->createImage(&imageInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), &mImage, &mMemory));
    if (result != VK_SUCCESS)
        return false;

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.format = format;
    viewInfo.image = mImage;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

	if (inParams.pixels)
	{
		CHECK_VK(result = setData(inParams.pixels, imageSize));
		if (result != VK_SUCCESS)
			return false;
	}

    CHECK_VK(result = vkCreateImageView(mDevice->handle(), &viewInfo, nullptr, &mImageView));
    if (result != VK_SUCCESS)
        return false;

    return true;
}

VkResult Texture2DVK::setData(const void* inData, uint64_t inSize)
{
    // TODO: This is crap
    VkResult result;
    VkBuffer stagingBuffer = nullptr;
    VkDeviceMemory stagingMemory = nullptr;
    void* dstData = nullptr;
    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = inSize;
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

    CHECK_VK(result = mDevice->copyBufferToImage(mImage, stagingBuffer, convertImageFormatVK(mFormat), mSize.x, mSize.y));
done:
    if (stagingBuffer)
        vkDestroyBuffer(mDevice->handle(), stagingBuffer, nullptr);
    if (stagingMemory)
        vkFreeMemory(mDevice->handle(), stagingMemory, nullptr);

    return result;
}
