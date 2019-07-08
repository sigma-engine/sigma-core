#pragma once

#include <sigma/Texture.hpp>

#include <glad/glad.h>

#include <cstdint>

class Texture2DGL : public Texture2D {
public:
    Texture2DGL();

    virtual ~Texture2DGL();

    GLuint handle() const { return mHandle; }

    bool initialize(ImageFormat inFormat, uint32_t inWidth, uint32_t inHeight, const void* inPixels);

private:
    GLuint mHandle = 0;
};