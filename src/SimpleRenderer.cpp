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
#include <sigma/UniformBuffer.hpp>
#include <sigma/VertexBuffer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <chrono>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
static std::vector<Vertex> vertices = {
    { { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    { { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
};
static std::vector<std::uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

struct SimpleUniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

SimpleRenderer::SimpleRenderer(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice, std::shared_ptr<Surface> inSurface)
    : mEngine(inEngine)
    , mDevice(inDevice)
    , mSurface(inSurface)
{
}

bool SimpleRenderer::initialize()
{
    auto setLayout = mDevice->createDescriptorSetLayout({ { 0, DescriptorType::UniformBuffer, 1 } });
    if (setLayout == nullptr)
        return false;

    for (uint32_t i = 0; i < mSurface->imageCount(); ++i) {
        auto commandBuffer = mDevice->createCommandBuffer();
        if (commandBuffer == nullptr)
            return false;

        auto uniformBuffer = mDevice->createUniformBuffer(sizeof(SimpleUniformBuffer));
        if (uniformBuffer == nullptr)
            return false;

        DescriptorSetCreateParams descriptorSetParams = {
            setLayout,
            { { 0, uniformBuffer } }
        };
        auto descriptorSet = mDevice->createDescriptorSet(descriptorSetParams);
        if (descriptorSet == nullptr)
            return false;

        mUniformBuffers.push_back(uniformBuffer);
        mDescriptorSets.push_back(descriptorSet);
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

    setupUniformBuffer(mUniformBuffers[imageData->imageIndex]);

    auto commandBuffer = mCommandBuffers[imageData->imageIndex];

    RenderPassBeginParams beginRenderPass{
        imageData->frameBuffer,
        imageData->frameBuffer->extent()
    };
    commandBuffer->begin();
    commandBuffer->beginRenderPass(beginRenderPass);
    commandBuffer->bindPipeline(mPipeline);
    commandBuffer->bindDescriptorSet(mDescriptorSets[imageData->imageIndex]);
    commandBuffer->bindVertexBuffer(mVertexBuffer);
    commandBuffer->bindIndexBuffer(mIndexBuffer);
    commandBuffer->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    commandBuffer->endRenderPass();
    commandBuffer->end();

    imageData->commandBuffers.push_back(commandBuffer);
    mSurface->presentImage(imageData);
}

void SimpleRenderer::setupUniformBuffer(std::shared_ptr<UniformBuffer> inBuffer)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto surfaceSize = mSurface->size();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    SimpleUniformBuffer ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), elapsedTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.projection = glm::perspective(glm::radians(45.0f), surfaceSize.x / float(surfaceSize.y), 0.1f, 10.0f);

    inBuffer->setData(static_cast<const void*>(&ubo), sizeof(SimpleUniformBuffer));

    SimpleUniformBuffer buffer;
}
