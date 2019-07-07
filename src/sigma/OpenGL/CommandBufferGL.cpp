#include <sigma/OpenGL/CommandBufferGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/DescriptorSetGL.hpp>
#include <sigma/OpenGL/IndexBufferGL.hpp>
#include <sigma/OpenGL/PipelineGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>
#include <sigma/OpenGL/VertexBufferGL.hpp>

#include <glad/glad.h>

void CommandBufferGL::begin()
{
}

void CommandBufferGL::beginRenderPass(const RenderPassBeginParams& inParams)
{
    CHECK_GL(glViewport(inParams.renderArea.origin.x, inParams.renderArea.origin.y, inParams.renderArea.size.x, inParams.renderArea.size.y));
    CHECK_GL(glClearColor(0, 0, 0, 1));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void CommandBufferGL::bindPipeline(std::shared_ptr<Pipeline> inPipeline)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<PipelineGL>(inPipeline), "Must use a opengl pipeline with a opengl command buffer!");
    auto pipeline = std::static_pointer_cast<PipelineGL>(inPipeline);
    if (pipeline != mPipeline)
        pipeline->bind();
    mPipeline = pipeline;
}

void CommandBufferGL::bindDescriptorSet(std::shared_ptr<DescriptorSet> inDescriptorSet)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<DescriptorSetGL>(inDescriptorSet), "Must use opengl descriptor set with opengl command buffer!");
    auto descriptorSet = std::static_pointer_cast<DescriptorSetGL>(inDescriptorSet);
    if (descriptorSet != mDescriptorSet)
        descriptorSet->bind();
    mDescriptorSet = descriptorSet;
}

void CommandBufferGL::bindVertexBuffer(std::shared_ptr<VertexBuffer> inBuffer)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<VertexBufferGL>(inBuffer), "Must use a opengl vertex buffer with a opengl command buffer!");
    auto vertexBuffer = std::static_pointer_cast<VertexBufferGL>(inBuffer);
    if (vertexBuffer != mVertexBuffer)
        vertexBuffer->bind();
    mVertexBuffer = vertexBuffer;
}

void CommandBufferGL::bindIndexBuffer(std::shared_ptr<IndexBuffer> inBuffer)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<IndexBufferGL>(inBuffer), "Must use a opengl index buffer with a opengl command buffer!");
    auto indexBuffer = std::static_pointer_cast<IndexBufferGL>(inBuffer);
    if (indexBuffer != mIndexBuffer) {
        indexBuffer->bind();
        mBoundIndexType = indexBuffer->dataType() == DataType::UShort ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
    }
    mIndexBuffer = indexBuffer;
}

void CommandBufferGL::draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance)
{
    CHECK_GL(glDrawArraysInstancedBaseInstance(GL_TRIANGLES, inFirstVertex, inVertexCount, inInstanceCount, inFirstInstance));
}

void CommandBufferGL::drawIndexed(uint32_t inIndexCount, uint32_t inInstanceCount, uint32_t inFirstIndex, int32_t inVertexOffset, uint32_t inFirstInstance)
{
    CHECK_GL(glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, inIndexCount, mBoundIndexType, nullptr, inInstanceCount, inFirstIndex, inFirstInstance));
}

void CommandBufferGL::endRenderPass()
{
}

void CommandBufferGL::end()
{
}
