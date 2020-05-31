#include <sigma/OpenGL/UniformBufferGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

#include <algorithm>

UniformBufferGL::UniformBufferGL()
{
	glCreateBuffers(1, &mHandle);
}

UniformBufferGL::~UniformBufferGL()
{
	glDeleteBuffers(1, &mHandle);
}

uint64_t UniformBufferGL::size() const
{
	return mSize;
}

void UniformBufferGL::setData(const void* inData, uint64_t inSize)
{
	CHECK_GL(glBindBuffer(GL_UNIFORM_BUFFER, mHandle));
	CHECK_GL(glBufferSubData(GL_UNIFORM_BUFFER, 0, std::min(inSize, mSize), inData));
	// TODO: check performance
	// void* dstData = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	// memcpy(dstData, inData, std::min(mSize, inSize));
	// glUnmapBuffer(GL_UNIFORM_BUFFER);
}

bool UniformBufferGL::initialize(uint64_t inSize)
{
	mSize = inSize;
	CHECK_GL(glBindBuffer(GL_UNIFORM_BUFFER, mHandle));
	CHECK_GL(glBufferData(GL_UNIFORM_BUFFER, mSize, nullptr, GL_DYNAMIC_DRAW));
	return true;
}