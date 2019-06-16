#pragma once

#include <sigma/DataTypes.hpp>

#include <glad/glad.h>

#include <cstdint>

std::size_t sizeOfDataType(DataType inType);

GLsizei componentCountOfDataType(DataType inType);

GLenum baseTypeOfDataType(DataType inType);
