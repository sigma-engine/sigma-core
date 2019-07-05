#include <sigma/Vulkan/PipelineVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>
#include <sigma/Vulkan/ShaderVK.hpp>
#include <sigma/Vulkan/UtilVK.hpp>

PipelineVK::PipelineVK(std::shared_ptr<DeviceVK> inDevice)
    : mDevice(inDevice)
{
}

PipelineVK::~PipelineVK()
{
    if (mDevice) {
        if (mPipeline)
            vkDestroyPipeline(mDevice->handle(), mPipeline, nullptr);
        if (mLayout)
            vkDestroyPipelineLayout(mDevice->handle(), mLayout, nullptr);
    }
}

bool PipelineVK::initialize(const PipelineCreateParams& inParams)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassVK>(inParams.renderPass), "Must use vulkan render pass with vulkan pipeline");
    mRenderPass = std::static_pointer_cast<RenderPassVK>(inParams.renderPass);

	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = inParams.vertexLayout.stride();
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(inParams.vertexLayout.attributeCount());
	uint32_t i = 0;
	for (const auto& member : inParams.vertexLayout) {
		attributeDescriptions[i].binding = 0;
		attributeDescriptions[i].location = i;
		attributeDescriptions[i].format = formatForDataTypeVK(member.type);
		attributeDescriptions[i].offset = member.offset;
		i++;
	}

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = float(inParams.viewportRect.origin.x);
    viewport.y = float(inParams.viewportRect.origin.y) + float(inParams.viewportRect.size.y);
    viewport.width = float(inParams.viewportRect.size.x);
    viewport.height = -float(inParams.viewportRect.size.y);

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = { inParams.viewportRect.size.x, inParams.viewportRect.size.y };

    VkPipelineViewportStateCreateInfo viewportStateInfo = {};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports = &viewport;
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f;
    rasterizerInfo.depthBiasClamp = 0.0f;
    rasterizerInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampleingInfo = {};
    multisampleingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleingInfo.sampleShadingEnable = VK_FALSE;
    multisampleingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleingInfo.minSampleShading = 1.0f;
    multisampleingInfo.pSampleMask = nullptr;
    multisampleingInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleingInfo.alphaToOneEnable = VK_FALSE;

    // VkPipelineDepthStencilStateCreateInfo

    VkPipelineColorBlendAttachmentState colorBlendAtt;
    colorBlendAtt.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAtt.blendEnable = VK_FALSE;
    colorBlendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAtt.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAtt.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAtt;
    colorBlendInfo.blendConstants[0] = 0.0f;
    colorBlendInfo.blendConstants[1] = 0.0f;
    colorBlendInfo.blendConstants[2] = 0.0f;
    colorBlendInfo.blendConstants[3] = 0.0f;

    //    std::vector<VkDynamicState> dynamicStates = {
    //        VK_DYNAMIC_STATE_VIEWPORT,
    //        VK_DYNAMIC_STATE_LINE_WIDTH
    //    };

    //    VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
    //    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    //    dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    //    dynamicStateInfo.pDynamicStates = dynamicStates.data();

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    for (const auto& shader : inParams.shaders) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<ShaderVK>(shader), "Must use vulkan shader to create vulkan pipeline!");
        auto shaderVk = std::static_pointer_cast<ShaderVK>(shader);
        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = convertShaderTypeVK(shader->type());
        shaderStageInfo.module = shaderVk->handle();
        shaderStageInfo.pName = "main";
        shaderStages.push_back(shaderStageInfo);
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(mDevice->handle(), &pipelineLayoutInfo, nullptr, &mLayout) != VK_SUCCESS) {
        return false;
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisampleingInfo;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = mLayout;
    pipelineInfo.renderPass = mRenderPass->handle();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(mDevice->handle(), nullptr, 1, &pipelineInfo, nullptr, &mPipeline) != VK_SUCCESS) {
        return false;
    }

    return true;
}
