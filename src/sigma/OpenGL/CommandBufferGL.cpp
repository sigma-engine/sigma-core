#include <sigma/OpenGL/CommandBufferGL.hpp>

#include <sigma/OpenGL/PipelineGL.hpp>
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

void CommandBufferGL::draw(uint32_t inVertexCount, uint32_t inInstanceCount, uint32_t inFirstVertex, uint32_t inFirstInstance)
{
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
}

void CommandBufferGL::endRenderPass()
{
}

void CommandBufferGL::end()
{
}
