#pragma once

#include <sigma/DataTypes.hpp>

#include <cstdint>
#include <string>
#include <vector>

class VertexLayout;

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

	virtual const VertexLayout& layout() const = 0;

	virtual void setData(const void* inData, uint64_t inSize) = 0;
};
