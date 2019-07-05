#pragma once

#include <cstdint>

enum class DataType {
    Float,
    Vec2,
    Vec3,
    Vec4,
    UInt,
    UShort
};

uint32_t sizeOfDataType(DataType inType);

uint32_t componentCountOfDataType(DataType inType);