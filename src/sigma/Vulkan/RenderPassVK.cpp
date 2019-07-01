#include <sigma/Vulkan/RenderPassVK.hpp>

#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

RenderPassVK::RenderPassVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

RenderPassVK::~RenderPassVK()
{
    if (mDevice && mRenderPass)
        vkDestroyRenderPass(mDevice->handle(), mRenderPass, nullptr);
}

bool RenderPassVK::initialize(const RenderPassCreateParams &inParams)
{
    std::vector<VkAttachmentDescription> colorAttachments;
    std::vector<VkAttachmentReference> colorAttachmentRefs;
    for(const auto &attch: inParams.attachments)
    {
        switch (attch.type) {
        case AttachmentType::ColorAttachment:
        {
            VkAttachmentDescription colorAtt = {};
            colorAtt.format = convertImageFormatVK(attch.format);
            colorAtt.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAtt.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAtt.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAtt.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAtt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAtt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAtt.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference attchRef = {};
            attchRef.attachment = colorAttachments.size();
            attchRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            colorAttachments.push_back(colorAtt);
            colorAttachmentRefs.push_back(attchRef);
            break;
        }
        }
    }

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = colorAttachmentRefs.size();
    subpass.pColorAttachments = colorAttachmentRefs.data();
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = colorAttachments.size();
    renderPassInfo.pAttachments = colorAttachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(mDevice->handle(), &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS)
    {
        return false;
    }

    return true;
}
