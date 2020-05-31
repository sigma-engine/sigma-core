#pragma once

#include <memory>
#include <vector>
#include <string>

#include <glm/mat4x4.hpp>

class Context;
class Device;
class Surface;
class Pipeline;
class VertexBuffer;
class IndexBuffer;
class CommandBuffer;
class UniformBuffer;
class DescriptorSet;
class Texture2D;
class Sampler2D;
class RenderPass;
class FrameBuffer;
struct TransformComponent;

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 uv;
};

class SimpleRenderer {
public:
	SimpleRenderer(std::shared_ptr<Context> inContext, std::shared_ptr<Surface> inSurface);

	bool initialize();

	void render(const TransformComponent& inCameraTransform);

private:
	std::shared_ptr<Context> mContext = nullptr;
	std::shared_ptr<Device> mDevice = nullptr;
	std::shared_ptr<Surface> mSurface = nullptr;

	std::shared_ptr<RenderPass> mTestRenderPass = nullptr;
	std::shared_ptr<Texture2D> mTestTexture0 = nullptr;
	std::shared_ptr<Texture2D> mTestTexture1 = nullptr;
	std::shared_ptr<FrameBuffer> mTestFrameBuffer = nullptr;

	std::vector<std::shared_ptr<CommandBuffer>> mCommandBuffers;
	std::vector<std::shared_ptr<UniformBuffer>> mUniformBuffers;
	std::vector<std::shared_ptr<DescriptorSet>> mDescriptorSets;

	std::shared_ptr<Pipeline> mPipeline = nullptr;
	std::shared_ptr<VertexBuffer> mVertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> mIndexBuffer = nullptr;
	std::shared_ptr<Texture2D> mTexture = nullptr;
	std::shared_ptr<Sampler2D> mSampler = nullptr;

	void setupUniformBuffer(const glm::mat4& inViewMatrix, std::shared_ptr<UniformBuffer> inBuffer);

	std::shared_ptr<Texture2D> loadTexture(const std::string& inFilepath);
};