#include <sigma/Vulkan/ShaderVK.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>

ShaderVK::ShaderVK(ShaderType inType, std::shared_ptr<DeviceVK> inDevice)
    : Shader(inType)
    , mDevice(inDevice)
{
}

ShaderVK::~ShaderVK()
{
    if (mModule && mDevice)
        vkDestroyShaderModule(mDevice->handle(), mModule, nullptr);
}

bool ShaderVK::initialize(const std::vector<uint32_t> &inByteCode)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = inByteCode.data();
    createInfo.codeSize = inByteCode.size() * sizeof(uint32_t);

    if (vkCreateShaderModule(mDevice->handle(), &createInfo, nullptr, &mModule) != VK_SUCCESS)
    {
        return false;
    }
    return true;
}
