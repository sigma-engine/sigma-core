#include <sigma/OpenGL/DataTypesGL.hpp>

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
