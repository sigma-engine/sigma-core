#include <sigma/OpenGL/SurfaceGL.hpp>

#include <sigma/OpenGL/CommandBufferGL.hpp>
#include <sigma/OpenGL/DeviceGL.hpp>
#include <sigma/OpenGL/FrameBufferGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

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
    return ImageFormat::UnormR8G8B8A8; // TODO: Don't hard code this?
}

uint32_t SurfaceGL::imageCount() const
{
    return 1;
}

std::shared_ptr<RenderPass> SurfaceGL::renderPass() const
{
    return mFrameData.frameBuffer->renderPass();
}

void SurfaceGL::nextImage(SurfaceImageData*& outData)
{
    outData = &mFrameData;
    outData->frameIndex = 0;
    outData->imageIndex = 0;
    outData->commandBuffers.clear();
}

bool SurfaceGL::createSwapChain(std::shared_ptr<DeviceGL> inDevice)
{
    RenderPassCreateParams renderPassCreateParams = {
        { { AttachmentType::ColorAttachment, format() } } // TODO: depth buffer, and stencil buffer
    };
    auto renderPass = std::static_pointer_cast<RenderPassGL>(inDevice->createRenderPass(renderPassCreateParams));
    if (renderPass == nullptr)
        return false;

    mFrameData.frameBuffer = std::make_shared<FrameBufferGL>(renderPass, glm::uvec2{ mWidth, mHeight }, 0);

    return mFrameData.frameBuffer != nullptr;
}
