#include <sigma/OpenGL/SurfaceGL.hpp>

#include <sigma/OpenGL/CommandBufferGL.hpp>
#include <sigma/OpenGL/DataTypesGL.hpp>
#include <sigma/OpenGL/DeviceGL.hpp>
#include <sigma/OpenGL/FrameBufferGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>

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
}

bool SurfaceGL::createSwapChain(std::shared_ptr<DeviceGL> inDevice)
{
    RenderPassCreateParams renderPassCreateParams = {
        { { AttachmentType::ColorAttachment, format() } }
    };
    auto renderPass = std::static_pointer_cast<RenderPassGL>(inDevice->createRenderPass(renderPassCreateParams));
    if (renderPass == nullptr)
        return false;

    mFrameData.frameBuffer = std::make_shared<FrameBufferGL>(renderPass, Rect<int32_t>{ { 0, 0 }, { mWidth, mHeight } });

    return mFrameData.frameBuffer != nullptr;
}
