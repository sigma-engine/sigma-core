#include <sigma/OpenGL/UtilGL.hpp>

#include <sigma/RenderPass.hpp>

#include <cassert>

GLenum baseTypeOfDataType(DataType inType)
{
    switch (inType) {
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
    assert(false && "Unknown DataType");
    return 0;
}

ImageFormat convertImageFormatGL(GLenum inFormat)
{
    return ImageFormat::Unknown;
}

std::string convertErrorStringGL(GLenum inError)
{
    switch (inError) {
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