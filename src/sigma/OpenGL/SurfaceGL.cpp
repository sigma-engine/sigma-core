#include <sigma/OpenGL/SurfaceGL.hpp>

#include <sigma/OpenGL/DataTypesGL.hpp>
#include <sigma/OpenGL/DeviceGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>
#include <sigma/OpenGL/CommandBufferGL.hpp>

bool SurfaceGL::initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight)
{
    mWidth = inWidth;
    mHeight = inHeight;
    return true;
}

glm::uvec2 SurfaceGL::size() const
{
    return { mWidth, mHeight };
}

ImageFormat SurfaceGL::format() const
{
    return convertImageFormatGL(mFormat);
}

std::shared_ptr<RenderPass> SurfaceGL::renderPass() const
{
    return mRenderPass;
}

void SurfaceGL::beginFrame(SurfaceImageData &outData)
{
	outData.commandBuffer = mCommandBuffer;
	outData.frameIndex = 0;
	outData.imageIndex = 0;
}

bool SurfaceGL::createSwapChain(std::shared_ptr<DeviceGL> inDevice)
{
    RenderPassCreateParams renderPassCreateParams = {
        { { AttachmentType::ColorAttachment, format() } }
    };
    mRenderPass = std::static_pointer_cast<RenderPassGL>(inDevice->createRenderPass(renderPassCreateParams));
	mCommandBuffer = std::static_pointer_cast<CommandBufferGL>(inDevice->createCommandBuffer());

    return mRenderPass != nullptr;
}
