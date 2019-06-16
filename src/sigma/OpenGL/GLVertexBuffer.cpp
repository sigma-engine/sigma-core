#include <sigma/OpenGL/GLVertexBuffer.hpp>
#include <sigma/OpenGL/GLDataTypes.hpp>

#include <glad/glad.h>

#include <cassert>

VertexLayout calculateLayout(const std::initializer_list<VertexMemberDescription> &inLayout)
{
    std::vector<VertexMember> members;
    std::size_t offset = 0;
    for(const auto &des: inLayout)
    {
        members.push_back({
            offset,
            sizeOfDataType(des.type),
            des.type,
            des.name,
            des.normalized
        });
        offset += members.back().size;
    }

    return {members, offset};
}

GLVertexBuffer::GLVertexBuffer(const std::initializer_list<VertexMemberDescription> &inLayout)
    : mLayout(calculateLayout(inLayout))
{
    glCreateVertexArrays(1, &mVAOHandle);
    glCreateBuffers(1, &mBufferHandle);

    glBindVertexArray(mVAOHandle);
    glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle);

    uint32_t i = 0;
    for(const auto &member: mLayout)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
            componentCountOfDataType(member.type),
            baseTypeOfDataType(member.type),
            member.normalized ? GL_TRUE : GL_FALSE,
            static_cast<GLsizei>(mLayout.stride()),
            reinterpret_cast<const void *>(member.offset)
        );
        i++;
    }
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteVertexArrays(1, &mVAOHandle);
    glDeleteBuffers(1, &mBufferHandle);
}

const VertexLayout &GLVertexBuffer::layout() const
{
    return mLayout;
}

void GLVertexBuffer::setData(const void *inData, std::size_t inSize)
{
    assert((inSize % mLayout.stride() == 0) && "Incorrect data for vertex buffer layout!");

    glBindBuffer(GL_ARRAY_BUFFER, mBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(inSize), inData, GL_STATIC_DRAW);
}

void GLVertexBuffer::bind() const
{
    glBindVertexArray(mVAOHandle);
}
