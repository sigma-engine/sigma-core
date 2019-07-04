#include <sigma/Vulkan/CommandBufferVK.hpp>

#include <sigma/Log.hpp>
#include <sigma/Vulkan/DeviceVK.hpp>
#include <sigma/Vulkan/FramebufferVK.hpp>
#include <sigma/Vulkan/RenderPassVK.hpp>

#include <limits>

CommandBufferVK::CommandBufferVK(std::shared_ptr<DeviceVK> inDevice, VkCommandPool inCommandPool)
    : mDevice(inDevice)
    , mCommandPool(inCommandPool)
{
}

CommandBufferVK::~CommandBufferVK()
{
    if (mDevice && mCommandPool && mBuffer) {
        if (mFence) {
            if (mInit)
                vkWaitForFences(mDevice->handle(), 1, &mFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
            vkDestroyFence(mDevice->handle(), mFence, nullptr);
        }
        vkFreeCommandBuffers(mDevice->handle(), mCommandPool, 1, &mBuffer);
    }
}

bool CommandBufferVK::initialize()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(mDevice->handle(), &allocInfo, &mBuffer) != VK_SUCCESS) {
        return false;
    }

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (vkCreateFence(mDevice->handle(), &fenceInfo, nullptr, &mFence) != VK_SUCCESS) {
        return false;
    }

    return true;
}

void CommandBufferVK::begin()
{
    if (mInit) {
        vkWaitForFences(mDevice->handle(), 1, &mFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(mDevice->handle(), 1, &mFence);
    } else {
        mInit = true;
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr;

    // TODO check for error
    vkBeginCommandBuffer(mBuffer, &beginInfo);
}

void CommandBufferVK::beginRenderPass(const RenderPassBeginParams& inParams)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<RenderPassVK>(inParams.renderPass), "Must use vulkan render pass with vulkan command buffer!");
    SIGMA_ASSERT(std::dynamic_pointer_cast<FramebufferVK>(inParams.frameBuffer), "Must use vulkan framebuffer with vulkan command buffer!");

    // TODO: correct clear colors!
    VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = std::static_pointer_cast<RenderPassVK>(inParams.renderPass)->handle();
    renderPassInfo.framebuffer = std::static_pointer_cast<FramebufferVK>(inParams.frameBuffer)->handle();
    renderPassInfo.renderArea.offset = { inParams.renderArea.origin.x, inParams.renderArea.origin.y };
    renderPassInfo.renderArea.extent = { inParams.renderArea.size.x, inParams.renderArea.size.y };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(mBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBufferVK::endRenderPass()
{
    vkCmdEndRenderPass(mBuffer);
}

void CommandBufferVK::end()
{
    // TODO check for error
    vkEndCommandBuffer(mBuffer);
}
