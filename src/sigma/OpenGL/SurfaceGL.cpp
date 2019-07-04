#include <sigma/OpenGL/SurfaceGL.hpp>

#include <sigma/OpenGL/DataTypesGL.hpp>
#include <sigma/OpenGL/DeviceGL.hpp>
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

std::shared_ptr<RenderPass> SurfaceGL::renderPass() const
{
    return mRenderPass;
}

std::shared_ptr<CommandBuffer> SurfaceGL::beginFrame()
{
    return nullptr;
}

void SurfaceGL::endFrame()
{
}

bool SurfaceGL::createRenderPass(std::shared_ptr<DeviceGL> inDevice)
{
    RenderPassCreateParams renderPassCreateParams = {
        { { AttachmentType::ColorAttachment, format() } }
    };
    mRenderPass = std::static_pointer_cast<RenderPassGL>(inDevice->createRenderPass(renderPassCreateParams));

    return mRenderPass != nullptr;
}
