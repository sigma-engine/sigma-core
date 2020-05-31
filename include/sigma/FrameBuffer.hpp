#pragma once

#include <sigma/Rect.hpp>

#include <memory>
#include <vector>

class RenderPass;
class Texture2D;

struct FrameBufferCreateParams {
	glm::uvec2 size;
	std::shared_ptr<RenderPass> renderPass = nullptr;
	std::vector<std::shared_ptr<Texture2D>> attachments;
};

class FrameBuffer {
public:
	virtual ~FrameBuffer() = default;

	virtual std::shared_ptr<RenderPass> renderPass() const = 0;

	virtual glm::uvec2 size() const = 0;
};