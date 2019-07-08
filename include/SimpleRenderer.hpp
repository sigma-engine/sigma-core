#pragma once

#include <memory>
#include <vector>

class Engine;
class Device;
class Surface;
class Pipeline;
class VertexBuffer;
class IndexBuffer;
class CommandBuffer;
class UniformBuffer;
class DescriptorSet;
class Texture2D;

class SimpleRenderer {
public:
    SimpleRenderer(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice, std::shared_ptr<Surface> inSurface);

    bool initialize();

    void render();

private:
    std::shared_ptr<Engine> mEngine = nullptr;
    std::shared_ptr<Device> mDevice = nullptr;
    std::shared_ptr<Surface> mSurface = nullptr;

    std::vector<std::shared_ptr<CommandBuffer>> mCommandBuffers;
    std::vector<std::shared_ptr<UniformBuffer>> mUniformBuffers;
    std::vector<std::shared_ptr<DescriptorSet>> mDescriptorSets;
	

    std::shared_ptr<Pipeline> mPipeline = nullptr;
    std::shared_ptr<VertexBuffer> mVertexBuffer = nullptr;
    std::shared_ptr<IndexBuffer> mIndexBuffer = nullptr;
	std::shared_ptr<Texture2D> mTexture = nullptr;

    void setupUniformBuffer(std::shared_ptr<UniformBuffer> inBuffer);

	std::shared_ptr<Texture2D> loadTexture(const std::string &inFilepath);
};