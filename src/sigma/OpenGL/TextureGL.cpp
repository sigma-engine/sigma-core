#include <sigma/OpenGL/TextureGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

Texture2DGL::Texture2DGL()
{
    CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &mHandle));
}

Texture2DGL::~Texture2DGL()
{
    CHECK_GL(glDeleteTextures(1, &mHandle));
}

bool Texture2DGL::initialize(ImageFormat inFormat, uint32_t inWidth, uint32_t inHeight, const void* inPixels)
{
    CHECK_GL(glBindTexture(GL_TEXTURE_2D, mHandle));
    CHECK_GL(glTexImage2D(GL_TEXTURE_2D,
        0,
        convertImageFormatInternalGL(inFormat),
        inWidth,
        inHeight,
        0,
        convertImageFormatFormatGL(inFormat),
        convertImageFormatTypeGL(inFormat),
        inPixels));
    return false;
}
