#pragma once

#include <sigma/Texture.hpp>

#include <vulkan/vulkan.h>

#include <cstdint>
#include <memory>

class DeviceVK;

class Texture2DVK : public Texture2D {
public:
    Texture2DVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~Texture2DVK();

    VkImage imageHandle() const { return mImage; };

    VkImageView imageViewHandle() const { return mImageView; };

    bool initialize(ImageFormat inFormat, uint32_t inWidth, uint32_t inHeight, const void* inPixels);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;

    uint32_t mWidth;
    uint32_t mHeight;
	ImageFormat mFormat;

    VkImage mImage = nullptr;
    VkDeviceMemory mMemory = nullptr; // TODO: memory manager
	VkImageView mImageView = nullptr;

    VkResult setData(const void* inData, uint64_t inSize);
};