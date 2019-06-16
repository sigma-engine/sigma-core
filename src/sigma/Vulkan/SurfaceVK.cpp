#include <sigma/Vulkan/SurfaceVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>

SurfaceVK::~SurfaceVK()
{
    if (mSurface && mInstance)
    {
        vkDestroySurfaceKHR(mInstance->handle(), mSurface, nullptr);
    }
}
