#include <SimpleRenderer.hpp>

#include <sigma/CommandBuffer.hpp>
#include <sigma/DescriptorSet.hpp>
#include <sigma/Device.hpp>
#include <sigma/Engine.hpp>
#include <sigma/FrameBuffer.hpp>
#include <sigma/IndexBuffer.hpp>
#include <sigma/Pipeline.hpp>
#include <sigma/Shader.hpp>
#include <sigma/Surface.hpp>
#include <sigma/VertexBuffer.hpp>

#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
static std::vector<Vertex> vertices = {
    { { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }
};
static std::vector<std::uint16_t> indices = { 0, 1, 2, 0, 2, 3 };

SimpleRenderer::SimpleRenderer(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice, std::shared_ptr<Surface> inSurface)
    : mEngine(inEngine)
    , mDevice(inDevice)
    , mSurface(inSurface)
{
}

bool SimpleRenderer::initialize()
{
    for (uint32_t i = 0; i < mSurface->imageCount(); ++i) {
        auto commandBuffer = mDevice->createCommandBuffer();
        if (commandBuffer == nullptr)
            return false;
        mCommandBuffers.push_back(commandBuffer);
    }

    VertexLayout vertexLayout = {
        { DataType::Vec3, "position" },
        { DataType::Vec3, "color" }
    };

    auto vertexShader = mDevice->createShader(ShaderType::VertexShader, "shaders/simple.vert");
    if (vertexShader == nullptr)
        return false;

    auto fragmentShader = mDevice->createShader(ShaderType::FragmentShader, "shaders/simple.frag");
    if (fragmentShader == nullptr)
        return false;

    auto setLayout = mDevice->createDescriptorSetLayout({ { DescriptorType::UniformBuffer, 1 } });
    if (setLayout == nullptr)
        return false;

    PipelineCreateParams pipelineParams = {
        { { 0, 0 }, mSurface->size() },
        mSurface->renderPass(),
        vertexLayout,
        { setLayout },
        { vertexShader, fragmentShader }
    };

    mPipeline = mDevice->createPipeline(pipelineParams);
    if (mPipeline == nullptr)
        return false;

    mVertexBuffer = mDevice->createVertexBuffer(vertexLayout, sizeof(Vertex) * vertices.size());
    if (mVertexBuffer == nullptr)
        return false;
    mVertexBuffer->setData(vertices.data(), sizeof(Vertex) * vertices.size());

    mIndexBuffer = mDevice->createIndexBuffer(PrimitiveType::Triangle, DataType::UShort, indices.size() * sizeof(uint16_t));
    if (mIndexBuffer == nullptr)
        return false;
    mIndexBuffer->setData(indices.data(), sizeof(uint16_t) * indices.size());

    return true;
}

void SimpleRenderer::render()
{
    SurfaceImageData* imageData = nullptr;
    mSurface->nextImage(imageData);

    auto commandBuffer = mCommandBuffers[imageData->imageIndex];

    RenderPassBeginParams beginRenderPass{
        imageData->frameBuffer,
        imageData->frameBuffer->extent()
    };

    commandBuffer->begin();
    commandBuffer->beginRenderPass(beginRenderPass);
    commandBuffer->bindPipeline(mPipeline);
    commandBuffer->bindVertexBuffer(mVertexBuffer);
    commandBuffer->bindIndexBuffer(mIndexBuffer);
    commandBuffer->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    commandBuffer->endRenderPass();
    commandBuffer->end();

    imageData->commandBuffers.push_back(commandBuffer);
    mSurface->presentImage(imageData);
}