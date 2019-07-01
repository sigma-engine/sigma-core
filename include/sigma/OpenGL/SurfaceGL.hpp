#pragma once

#include <sigma/Surface.hpp>

#include <glad/glad.h>

class SurfaceGL : public Surface {
public:
    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) override;

    virtual glm::uvec2 size() const override;

    virtual ImageFormat format() const override;
protected:
    uint32_t mWidth;
    uint32_t mHeight;
    GLenum mFormat;
};
