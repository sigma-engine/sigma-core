#pragma once

#include <sigma/Surface.hpp>

class SurfaceGL : public Surface {
public:
    virtual bool initialize(std::shared_ptr<Device> inDevice) override;
};