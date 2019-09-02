#include <sigma/OpenGL/UtilGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/Texture.hpp>
#include <sigma/Sampler.hpp>

GLenum baseTypeOfDataType(DataType inType)
{
    switch (inType)
    {
    case DataType::Float:
        return GL_FLOAT;
    case DataType::Vec2:
        return GL_FLOAT;
    case DataType::Vec3:
        return GL_FLOAT;
    case DataType::Vec4:
        return GL_FLOAT;
    case DataType::UInt:
        return GL_UNSIGNED_INT;
    case DataType::UShort:
        return GL_UNSIGNED_SHORT;
    }
    SIGMA_ASSERT(false, "Unknown DataType");
    return 0;
}

GLenum convertImageFormatInternalGL(ImageFormat inFormat)
{
    switch (inFormat)
    {
    case ImageFormat::UnormB8G8R8A8:
    case ImageFormat::UnormR8G8B8A8:
        return GL_RGBA8;
    default:
        break;
    }
    SIGMA_ASSERT(false, "Unknown ImageFormat");
    return GL_RGBA;
}

GLenum convertImageFormatFormatGL(ImageFormat inFormat)
{
    switch (inFormat)
    {
    case ImageFormat::UnormB8G8R8A8:
        return GL_BGRA;
    case ImageFormat::UnormR8G8B8A8:
        return GL_RGBA;
    default:
        break;
    }
    SIGMA_ASSERT(false, "Unknown ImageFormat");
    return GL_RGBA_INTEGER;
}

GLenum convertImageFormatTypeGL(ImageFormat inFormat)
{
    switch (inFormat)
    {
    case ImageFormat::UnormB8G8R8A8:
    case ImageFormat::UnormR8G8B8A8:
        return GL_UNSIGNED_BYTE;
    default:
        break;
    }
    SIGMA_ASSERT(false, "Unknown ImageFormat");
    return GL_UNSIGNED_BYTE;
}

std::string convertErrorStringGL(GLenum inError)
{
    switch (inError)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_CONTEXT_LOST:
        return "GL_CONTEXT_LOST";
    default:
        break;
    }
    return "UNKNOWN_ERROR";
}

GLenum convertSamplerWarpModeGL(SamplerWarpMode inMode)
{
    switch (inMode)
    {
    case SamplerWarpMode::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case SamplerWarpMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case SamplerWarpMode::Repeat:
        return GL_REPEAT;
    }
    SIGMA_ASSERT(false, "Unknown SamplerWarpMode");
    return GL_REPEAT;
}

GLenum convertSamplerFilterModeGL(SamplerFilterMode inFilterMode, SamplerMipmapMode inMipmapMode)
{
    switch (inMipmapMode)
    {
    case SamplerMipmapMode::None:
        switch (inFilterMode)
        {
        case SamplerFilterMode::Nearest:
            return GL_NEAREST;
        case SamplerFilterMode::Linear:
            return GL_LINEAR;
        }
        break;
    case SamplerMipmapMode::Nearest:
        switch (inFilterMode)
        {
        case SamplerFilterMode::Nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case SamplerFilterMode::Linear:
            return GL_LINEAR_MIPMAP_NEAREST;
        }
        break;
    case SamplerMipmapMode::Linear:
        switch (inFilterMode)
        {
        case SamplerFilterMode::Nearest:
            return GL_NEAREST_MIPMAP_LINEAR;
        case SamplerFilterMode::Linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
        break;
    }
    SIGMA_ASSERT(false, "Unknown SamplerFilterMode, SamplerMipmapMode combo");
    return GL_LINEAR;
}