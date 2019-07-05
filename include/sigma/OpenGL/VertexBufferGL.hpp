#pragma once

#include <sigma/VertexBuffer.hpp>
#include <sigma/VertexLayout.hpp>

#include <cstdint>

class VertexBufferGL : public VertexBuffer {
public:
    VertexBufferGL(const VertexLayout& inLayout);

    virtual ~VertexBufferGL();

    virtual const VertexLayout& layout() const override;

    virtual void setData(const void* inData, uint64_t inSize) override;

    void bind() const;

private:
    VertexLayout mLayout;
    uint32_t mVAOHandle = 0;
    uint32_t mBufferHandle = 0;
};
