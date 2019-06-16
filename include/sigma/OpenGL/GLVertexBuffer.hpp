#pragma once

#include <sigma/VertexBuffer.hpp>

#include <cstdint>

class GLVertexBuffer : public VertexBuffer
{
public:
    GLVertexBuffer(const std::initializer_list<VertexMemberDescription> &inLayout);

	virtual ~GLVertexBuffer();

    virtual const VertexLayout &layout() const override;

    virtual void setData(const void *inData, std::size_t inSize) override;

    void bind() const;
private:
    VertexLayout mLayout;
    uint32_t mVAOHandle = 0;
    uint32_t mBufferHandle = 0;
};
