#pragma once

#include <sigma/DataTypes.hpp>
#include <sigma/Log.hpp>

#include <glad/glad.h>

#include <cstdint>
#include <string>

// clang-format off
#ifdef NDEBUG
#define CHECK_GL(expression, ...) {(expression);}
#else
#define CHECK_GL(expression, ...) {(expression); auto error = glGetError(); SIGMA_ASSERT(error == GL_NO_ERROR, "OpenGL Error: {} (0x{:x})", convertErrorStringGL(error), error);}
#endif
// clang-format on

enum class ImageFormat;

GLenum baseTypeOfDataType(DataType inType);

ImageFormat convertImageFormatGL(GLenum inFormat);

GLenum convertImageFormatGL(ImageFormat inFormat);

std::string convertErrorStringGL(GLenum inError);
