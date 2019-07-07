#include <sigma/OpenGL/IndexBufferGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

#include <cassert>

uint64_t primitiveComponentCount(PrimitiveType inType)
{
    switch (inType) {
    case PrimitiveType::Triangle:
        return 3;
    }
    SIGMA_ASSERT(false, "Unknown PrimitiveType!");
    return 0;
}

GLenum glEnumForPrimitive(PrimitiveType inType)
{
    switch (inType) {
    case PrimitiveType::Triangle:
        return GL_TRIANGLES;
    }
    SIGMA_ASSERT(false, "Unknown PrimitiveType!");
    return GL_INVALID_ENUM;
}

IndexBufferGL::IndexBufferGL(PrimitiveType inPrimitiveType, DataType inDataType)
    : mPrimitiveType(inPrimitiveType)
    , mDataType(inDataType)
    , mCount(0)
{
    CHECK_GL(glCreateBuffers(1, &mHandle));
}

IndexBufferGL::~IndexBufferGL()
{
    CHECK_GL(glDeleteBuffers(1, &mHandle));
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
    SIGMA_ASSERT(inSize % (primitiveComponentCount(mPrimitiveType) * sizeOfDataType(mDataType)) == 0, "Data size must be a multiple of stride!");
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
    CHECK_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(inSize), inData, GL_STATIC_DRAW));
    mCount = inSize / sizeOfDataType(mDataType);
}

void IndexBufferGL::bind()
{
    CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
}
