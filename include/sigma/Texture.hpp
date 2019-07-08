#pragma once

#include <cstdint>

enum class ImageFormat {
    Unknown,
    UnormB8G8R8A8,
    UnormR8G8B8A8
};

uint64_t imageFormatPixelSize(ImageFormat inFormat);

class Texture2D {
public:
    virtual ~Texture2D() = default;
};