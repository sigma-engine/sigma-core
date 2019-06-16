#pragma once

#include <sigma/IndexBuffer.hpp>

class GLIndexBuffer : public IndexBuffer 
{
public:
    GLIndexBuffer(PrimitiveType inPrimitiveType, DataType inDataType);

	virtual ~GLIndexBuffer();

    virtual DataType dataType() const override;

    virtual PrimitiveType primitiveType() const override;

    virtual void setData(const void *inData, std::size_t inSize) override;

    void draw();
private:
    PrimitiveType mPrimitiveType;
    DataType mDataType;
    std::size_t mCount;
    uint32_t mHandle;
};
