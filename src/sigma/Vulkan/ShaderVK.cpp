#include <sigma/Vulkan/ShaderVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

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

bool ShaderVK::initialize(const std::vector<uint32_t>& inByteCode)
{
    VkResult result;
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = inByteCode.data();
    createInfo.codeSize = inByteCode.size() * sizeof(uint32_t);

    CHECK_VK(result = vkCreateShaderModule(mDevice->handle(), &createInfo, nullptr, &mModule));
    return result == VK_SUCCESS;
}
