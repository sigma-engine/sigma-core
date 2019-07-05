#pragma once

#include <sigma/DataTypes.hpp>

#include <glad/glad.h>

#include <cstdint>

enum class ImageFormat;

GLenum baseTypeOfDataType(DataType inType);

ImageFormat convertImageFormatGL(GLenum inFormat);

GLenum convertImageFormatGL(ImageFormat inFormat);
