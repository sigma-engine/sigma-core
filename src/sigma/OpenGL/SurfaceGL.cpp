#include <sigma/OpenGL/SurfaceGL.hpp>

#include <sigma/OpenGL/DataTypesGL.hpp>

bool SurfaceGL::initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight)
{
    mWidth = inWidth;
    mHeight = inHeight;
    return true;
}

glm::uvec2 SurfaceGL::size() const
{
    return {mWidth, mHeight};
}

ImageFormat SurfaceGL::format() const
{
    return convertImageFormatGL(mFormat);
}
