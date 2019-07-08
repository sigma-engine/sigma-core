#pragma once

#include <cstdint>

class UniformBuffer {
public:
    virtual ~UniformBuffer() = default;

    virtual uint64_t size() const = 0;

    virtual void setData(const void* inData, uint64_t inSize) = 0;
};