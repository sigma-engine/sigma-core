#pragma once

#include <sigma/IndexBuffer.hpp>

class IndexBufferGL : public IndexBuffer {
public:
    IndexBufferGL(PrimitiveType inPrimitiveType, DataType inDataType);

    virtual ~IndexBufferGL();

    virtual DataType dataType() const override;

    virtual PrimitiveType primitiveType() const override;

    virtual void setData(const void* inData, size_t inSize) override;

    void draw();

private:
    PrimitiveType mPrimitiveType;
    DataType mDataType;
    size_t mCount;
    uint32_t mHandle;
};
