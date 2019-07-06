#include <sigma/OpenGL/IndexBufferGL.hpp>

#include <sigma/OpenGL/DataTypesGL.hpp>

#include <glad/glad.h>

#include <cassert>

uint64_t primitiveComponentCount(PrimitiveType inType)
{
    switch (inType) {
    case PrimitiveType::Triangle:
        return 3;
    }
    assert(false && "Unknown PrimitiveType!");
    return 0;
}

GLenum glEnumForPrimitive(PrimitiveType inType)
{
    switch (inType) {
    case PrimitiveType::Triangle:
        return GL_TRIANGLES;
    }
    assert(false && "Unknown PrimitiveType!");
    return GL_INVALID_ENUM;
}

IndexBufferGL::IndexBufferGL(PrimitiveType inPrimitiveType, DataType inDataType)
    : mPrimitiveType(inPrimitiveType)
    , mDataType(inDataType)
    , mCount(0)
{
    glCreateBuffers(1, &mHandle);
}

IndexBufferGL::~IndexBufferGL()
{
    glDeleteBuffers(1, &mHandle);
}

DataType IndexBufferGL::dataType() const
{
    return mDataType;
}

PrimitiveType IndexBufferGL::primitiveType() const
{
    return mPrimitiveType;
}

void IndexBufferGL::setData(const void* inData, uint64_t inSize)
{
    assert(inSize % (primitiveComponentCount(mPrimitiveType) * sizeOfDataType(mDataType)) == 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(inSize), inData, GL_STATIC_DRAW);
    mCount = inSize / sizeOfDataType(mDataType);
}

void IndexBufferGL::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
}
