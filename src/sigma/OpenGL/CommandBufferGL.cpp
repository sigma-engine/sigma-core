#include <sigma/OpenGL/CommandBufferGL.hpp>

#include <sigma/OpenGL/PipelineGL.hpp>
#include <sigma/OpenGL/VertexBufferGL.hpp>
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

void CommandBufferGL::draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance)
{
	// glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void CommandBufferGL::endRenderPass()
{
}

void CommandBufferGL::end()
{
}
