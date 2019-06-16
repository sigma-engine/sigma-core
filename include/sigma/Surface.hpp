#pragma once

#include <memory>

class Device;

class Surface {
public:
    virtual ~Surface() = default;

    virtual bool initialize(std::shared_ptr<Device> inDevice) = 0;
};