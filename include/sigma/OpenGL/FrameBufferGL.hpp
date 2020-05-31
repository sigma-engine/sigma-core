#pragma once

#include <sigma/FrameBuffer.hpp>

#include <glad/glad.h>

#include <vector>
#include <memory>

class DeviceGL;
class RenderPassGL;
class Texture2DGL;

class FrameBufferGL : public FrameBuffer {
public:
	FrameBufferGL(std::shared_ptr<RenderPassGL> inRenderPass, const glm::uvec2& inSize, GLuint inHandle);

	FrameBufferGL();

	virtual ~FrameBufferGL();

	std::shared_ptr<RenderPass> renderPass() const override;

	virtual glm::uvec2 size() const override;

	bool initialize(const FrameBufferCreateParams& inParams);

	void bind();

private:
	std::shared_ptr<RenderPassGL> mRenderPass = nullptr;
	std::vector<std::shared_ptr<Texture2DGL>> mAttachments;
	std::vector<GLenum> mDrawBuffers;
	glm::uvec2 mSize;
	GLuint mHandle = 0;
};