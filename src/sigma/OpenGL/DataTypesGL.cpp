#include <sigma/OpenGL/DataTypesGL.hpp>

#include <sigma/RenderPass.hpp>

#include <cassert>

std::size_t sizeOfDataType(DataType inType)
{
    switch (inType)
    {
    case DataType::Float:   return 1 * sizeof(GLfloat);
    case DataType::Vec2:    return 2 * sizeof(GLfloat);
    case DataType::Vec3:    return 3 * sizeof(GLfloat);
    case DataType::Vec4:    return 4 * sizeof(GLfloat);
    case DataType::UInt:    return 1 * sizeof(GLuint);
    case DataType::UShort:  return 1 * sizeof(GLushort);
    }
    assert(false && "Unknown DataType");
    return 0;
}

GLsizei componentCountOfDataType(DataType inType)
{
    switch (inType)
    {
    case DataType::Float:   return 1;
    case DataType::Vec2:    return 2;
    case DataType::Vec3:    return 3;
    case DataType::Vec4:    return 4;
    case DataType::UInt:    return 1;
    case DataType::UShort:  return 1;
    }
    assert(false && "Unknown DataType");
    return 0;
}

GLenum baseTypeOfDataType(DataType inType)
{
    switch (inType)
    {
    case DataType::Float:   return GL_FLOAT;
    case DataType::Vec2:    return GL_FLOAT;
    case DataType::Vec3:    return GL_FLOAT;
    case DataType::Vec4:    return GL_FLOAT;
    case DataType::UInt:    return GL_UNSIGNED_INT;
    case DataType::UShort:  return GL_UNSIGNED_SHORT;
    }
    assert(false && "Unknown DataType");
    return 0;
}

ImageFormat convertImageFormatGL(GLenum inFormat)
{
    return ImageFormat::Unknown;
}
