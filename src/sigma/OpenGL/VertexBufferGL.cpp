#include <sigma/OpenGL/VertexBufferGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

#include <cassert>

VertexBufferGL::VertexBufferGL(const VertexLayout& inLayout)
	: mLayout(inLayout)
{
	CHECK_GL(glCreateVertexArrays(1, &mVAOHandle));
	CHECK_GL(glCreateBuffers(1, &mBufferHandle));

	CHECK_GL(glBindVertexArray(mVAOHandle));
	CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle));

	for (const auto& member : mLayout) {
		CHECK_GL(glEnableVertexAttribArray(member.location));
		CHECK_GL(glVertexAttribPointer(member.location,
			componentCountOfDataType(member.type),
			baseTypeOfDataType(member.type),
			member.normalized ? GL_TRUE : GL_FALSE,
			static_cast<GLsizei>(mLayout.stride()),
			reinterpret_cast<const void*>(static_cast<uint64_t>(member.offset))));
	}
}

VertexBufferGL::~VertexBufferGL()
{
	glDeleteVertexArrays(1, &mVAOHandle);
	glDeleteBuffers(1, &mBufferHandle);
}

const VertexLayout& VertexBufferGL::layout() const
{
	return mLayout;
}

void VertexBufferGL::setData(const void* inData, std::size_t inSize)
{
	assert((inSize % mLayout.stride() == 0) && "Incorrect data for vertex buffer layout!");

	glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(inSize), inData, GL_STATIC_DRAW);
}

void VertexBufferGL::bind() const
{
	glBindVertexArray(mVAOHandle);
}
