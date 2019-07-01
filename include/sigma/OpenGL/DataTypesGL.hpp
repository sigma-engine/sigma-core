#pragma once

#include <sigma/DataTypes.hpp>

#include <glad/glad.h>

#include <cstdint>

enum class ImageFormat;

std::size_t sizeOfDataType(DataType inType);

GLsizei componentCountOfDataType(DataType inType);

GLenum baseTypeOfDataType(DataType inType);

ImageFormat convertImageFormatGL(GLenum inFormat);

GLenum convertImageFormatGL(ImageFormat inFormat);
