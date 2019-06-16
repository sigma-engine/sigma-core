#pragma once

#include <sigma/DataTypes.hpp>

#include <cstdint>

enum class PrimitiveType
{
	Triangle
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() = default;

    virtual DataType dataType() const = 0;

    virtual PrimitiveType primitiveType() const = 0;

	virtual void setData(const void *inData, std::size_t inSize) = 0;
};
