#pragma once

#include <sigma/DataTypes.hpp>
#include <sigma/Log.hpp>

#include <cstdint>
#include <string>

// clang-format off
#ifdef NDEBUG
#define CHECK_GL(expression) {(expression);}
#else
#define CHECK_GL(expression) {(expression); auto error = glGetError(); SIGMA_ASSERT(error == GL_NO_ERROR, "OpenGL Error: {} (0x{:x})", convertErrorStringGL(error), error);}
#endif
// clang-format on

enum class ImageFormat;
enum class SamplerWarpMode;
enum class SamplerFilterMode;
enum class SamplerMipmapMode;

unsigned int baseTypeOfDataType(DataType inType);

unsigned int convertImageFormatInternalGL(ImageFormat inFormat);

unsigned int convertImageFormatFormatGL(ImageFormat inFormat);

unsigned int convertImageFormatTypeGL(ImageFormat inFormat);

std::string convertErrorStringGL(unsigned int inError);

unsigned int convertSamplerWarpModeGL(SamplerWarpMode inMode);

unsigned int convertSamplerFilterModeGL(SamplerFilterMode inFilterMode, SamplerMipmapMode inMipmapMode);
