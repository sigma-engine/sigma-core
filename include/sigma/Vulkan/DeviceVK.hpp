#pragma once

#include <sigma/Device.hpp>

#include <sigma/Vulkan/DeviceManagerVK.hpp>

#include <optional>
#include <set>
#include <string>

class SurfaceVK;

struct SurfaceSwapChainInfoVK
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> modes;
    std::optional<uint32_t> presentFamily;
};


class DeviceVK : public Device, public std::enable_shared_from_this<DeviceVK> {
public:
    DeviceVK(VkInstance inInstance, VkPhysicalDevice inDevice, const std::vector<std::string> &inEnabledLayers);

    virtual ~DeviceVK();

    virtual DeviceType type() const override;

    virtual uint32_t maxGraphicsQueues() const override;

    virtual uint32_t maxComputeQueues() const override;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const override;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces) override;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string &inCode) override;

    virtual std::shared_ptr<Program> createProgram(const std::vector<std::shared_ptr<Shader> > &inShaders) override;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const std::initializer_list<VertexMemberDescription> &inLayout) override;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType) override;

    virtual void draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer) override;

    uint32_t graphicsQueueFamily() const;

    VkDevice handle() const { return mDevice; }
private:
    VkInstance mInstance = nullptr;
    VkPhysicalDevice mPhysicalDevice = nullptr;
    VkDevice mDevice = nullptr;
    std::vector<std::string> mRequiredExtensions;
    std::vector<std::string> mEnabledLayers;
    VkPhysicalDeviceProperties mPhysicalDeviceProperties;
    std::vector<VkExtensionProperties> mExtensionProperties;
    std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
    VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
    std::optional<uint32_t> mGraphicsFamily;
    std::optional<uint32_t> mComputeFamily;

    std::optional<SurfaceSwapChainInfoVK> getSwapChainInfo(std::shared_ptr<SurfaceVK> inSurface) const;
};
