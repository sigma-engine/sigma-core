#pragma once

#include <sigma/UniformBuffer.hpp>

#include <glad/glad.h>

class UniformBufferGL : public UniformBuffer {
public:
    UniformBufferGL();

    virtual ~UniformBufferGL();

	virtual uint64_t size() const override;

    virtual void setData(const void* inData, uint64_t inSize) override;

	GLuint handle() const { return mHandle; }

    bool initialize(uint64_t inSize);

private:
    GLuint mHandle;
    uint64_t mSize;
};