#pragma once

#include <sigma/Shader.hpp>

#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

class DeviceVK;

class ShaderVK : public Shader {
public:
    ShaderVK(ShaderType inType, std::shared_ptr<DeviceVK> inDevice);

    virtual ~ShaderVK();

    VkShaderModule handle() const { return mModule; }

    bool initialize(const std::vector<uint32_t> &inByteCode);
private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    VkShaderModule mModule = nullptr;
};
