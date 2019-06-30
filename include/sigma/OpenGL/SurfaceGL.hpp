#pragma once

#include <sigma/Surface.hpp>

class SurfaceGL : public Surface {
public:
    virtual bool initialize(std::shared_ptr<DeviceManager> inDevice, uint32_t inWidth, uint32_t inHeight) override;
private:
    uint32_t mWidth;
    uint32_t mHeight;
};
