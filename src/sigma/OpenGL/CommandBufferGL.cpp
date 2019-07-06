#include <sigma/OpenGL/CommandBufferGL.hpp>

#include <sigma/OpenGL/PipelineGL.hpp>
#include <sigma/OpenGL/VertexBufferGL.hpp>
#include <sigma/OpenGL/IndexBufferGL.hpp>
#include <sigma/Log.hpp>

#include <glad/glad.h>

void CommandBufferGL::begin()
{
}

void CommandBufferGL::beginRenderPass(const RenderPassBeginParams& inParams)
{
}

void CommandBufferGL::bindPipeline(std::shared_ptr<Pipeline> inPipeline)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<PipelineGL>(inPipeline), "Must use a opengl pipeline with a opengl command buffer!");
	auto pipeline = std::static_pointer_cast<PipelineGL>(inPipeline);
	pipeline->bind();
}

void CommandBufferGL::bindVertexBuffer(std::shared_ptr<VertexBuffer> inBuffer)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<VertexBufferGL>(inBuffer), "Must use a opengl vertex buffer with a opengl command buffer!");
	auto vertexBuffer = std::static_pointer_cast<VertexBufferGL>(inBuffer);
	vertexBuffer->bind();
}

void CommandBufferGL::bindIndexBuffer(std::shared_ptr<IndexBuffer> inBuffer)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<IndexBufferGL>(inBuffer), "Must use a opengl index buffer with a opengl command buffer!");
	auto indexBuffer = std::static_pointer_cast<IndexBufferGL>(inBuffer);
	indexBuffer->bind();
	mBoundIndexType = indexBuffer->dataType() == DataType::UShort ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
}

void CommandBufferGL::draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance)
{
	glDrawArraysInstancedBaseInstance(GL_TRIANGLES, inFirstVertex, inVertexCount, inInstanceCount, inFirstInstance);
}

void CommandBufferGL::drawIndexed(uint32_t inIndexCount, uint32_t inInstanceCount, uint32_t inFirstIndex, int32_t inVertexOffset, uint32_t inFirstInstance)
{
	glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, inIndexCount, mBoundIndexType, nullptr, inInstanceCount, inFirstIndex, inFirstInstance);
}

void CommandBufferGL::endRenderPass()
{
}

void CommandBufferGL::end()
{
}
