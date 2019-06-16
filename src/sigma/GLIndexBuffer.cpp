#include <sigma/GLIndexBuffer.hpp>
#include <sigma/GLDataTypes.hpp>

#include <glad/glad.h>

#include <cassert>

std::size_t primitiveComponentCount(PrimitiveType inType)
{
    switch (inType)
    {
    case PrimitiveType::Triangle: return 3;
    }
    assert(false && "Unknown PrimitiveType!");
    return 0;
}

GLenum glEnumForPrimitive(PrimitiveType inType)
{
    switch (inType)
    {
    case PrimitiveType::Triangle: return GL_TRIANGLES;
    }
    assert(false && "Unknown PrimitiveType!");
    return GL_INVALID_ENUM;
}

GLIndexBuffer::GLIndexBuffer(PrimitiveType inPrimitiveType, DataType inDataType)
    : mPrimitiveType(inPrimitiveType)
    , mDataType(inDataType)
    , mCount(0)
{
    glCreateBuffers(1, &mHandle);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &mHandle);
}

DataType GLIndexBuffer::dataType() const
{
    return mDataType;
}

PrimitiveType GLIndexBuffer::primitiveType() const
{
    return mPrimitiveType;
}

void GLIndexBuffer::setData(const void *inData, std::size_t inSize)
{
    assert(inSize % (primitiveComponentCount(mPrimitiveType) * sizeOfDataType(mDataType)) == 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(inSize), inData, GL_STATIC_DRAW);
    mCount = inSize / sizeOfDataType(mDataType);
}

void GLIndexBuffer::draw()
{
    if (mCount)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        glDrawElements(glEnumForPrimitive(mPrimitiveType), static_cast<GLsizei>(mCount), baseTypeOfDataType(mDataType), nullptr);
    }
}
