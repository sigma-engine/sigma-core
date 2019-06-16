#pragma once

#include <sigma/VertexBuffer.hpp>

#include <cstdint>

class VertexBufferGL : public VertexBuffer {
public:
    VertexBufferGL(const std::initializer_list<VertexMemberDescription>& inLayout);

    virtual ~VertexBufferGL();

    virtual const VertexLayout& layout() const override;

    virtual void setData(const void* inData, std::size_t inSize) override;

    void bind() const;

private:
    VertexLayout mLayout;
    uint32_t mVAOHandle = 0;
    uint32_t mBufferHandle = 0;
};
