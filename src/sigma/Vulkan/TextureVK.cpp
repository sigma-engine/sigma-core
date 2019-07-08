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

bool Texture2DVK::initialize(ImageFormat inFormat, uint32_t inWidth, uint32_t inHeight, const void* inPixels)
{
    uint64_t imageSize = inWidth * inHeight * imageFormatPixelSize(inFormat);

    VkResult result;
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = inWidth;
    imageInfo.extent.height = inHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = convertImageFormatVK(inFormat);
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    CHECK_VK(result = mDevice->createImage(&imageInfo, VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), &mImage, &mMemory));
    if (result != VK_SUCCESS)
        return false;

    // TODO: Image View

    setData(inPixels, imageSize);

    mWidth = inWidth;
    mHeight = inHeight;

    return false;
}

bool Texture2DVK::setData(const void* inData, uint64_t inSize)
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

    CHECK_VK(result = mDevice->copyBufferToImage(mImage, stagingBuffer, mWidth, mHeight));

done:
    if (stagingBuffer)
        vkDestroyBuffer(mDevice->handle(), stagingBuffer, nullptr);
    if (stagingMemory)
        vkFreeMemory(mDevice->handle(), stagingMemory, nullptr);

    return result == VK_SUCCESS;
}
