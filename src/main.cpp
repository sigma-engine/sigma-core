#include <sigma/Engine.hpp>
#include <sigma/Window.hpp>
#include <sigma/Device.hpp>
#include <sigma/DeviceManager.hpp>
#include <sigma/Shader.hpp>
#include <sigma/VertexBuffer.hpp>
#include <sigma/IndexBuffer.hpp>
#include <sigma/Pipeline.hpp>
#include <sigma/RenderPass.hpp>
#include <sigma/Surface.hpp>
#include <glm/vec3.hpp>
#include <sigma/Log.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
};
static std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f},{1.0f, 1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f},{1.0f, 1.0f, 0.0f}},
    {{0.0f, 0.5f, 0.0f},{1.0f, 1.0f, 0.0f}}
};
static std::vector<std::uint16_t> indices = {0, 1, 2};

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    auto engine = Engine::create();
    if (!engine->initialize(GraphicsAPI::Vulkan))
        return -1;

    auto deviceManager = engine->deviceManager();
    if (deviceManager == nullptr)
        return -1;

    auto window = engine->createWindow("Simple Game", 1920 / 2, 1080 / 2);
    if (window == nullptr)
        return -1;

    std::vector<std::shared_ptr<Device>> surfaceDevices;
    deviceManager->enumerateSurfaceDevices(window->surface(), surfaceDevices);
    if (surfaceDevices.empty())
        return -1;

    auto device = surfaceDevices[0];
    if (!device->initialize({window->surface()}))
        return -1;

    auto vertexShader = device->createShader(ShaderType::VertexShader, "shaders/simple.vert");
    if (!vertexShader)
        return -1;

    auto fragmentShader = device->createShader(ShaderType::FragmentShader, "shaders/simple.frag");
    if (!fragmentShader)
        return -1;

    RenderPassCreateParams renderPassParams = {
        {{AttachmentType::ColorAttachment, window->surface()->format()}}
    };
    auto renderPass = device->createRenderPass(renderPassParams);
    if (!renderPass)
        return -1;

    PipelineCreateParams pipelineParams = {
        {{0,0}, window->surface()->size()},
        renderPass,
        {vertexShader, fragmentShader}
    };
    auto pipeline = device->createPipeline(pipelineParams);
    if (!pipeline)
        return -1;
    
//    auto program = device->createProgram({vertexShader, fragmentShader});

//    auto vertexBuffer = device->createVertexBuffer({
//        {DataType::Vec3, "position"},
//        {DataType::Vec3, "color"}
//    });
//    vertexBuffer->setData(vertices.data(), sizeof(Vertex) * vertices.size());
    
//    auto indexBuffer = device->createIndexBuffer(PrimitiveType::Triangle, DataType::UShort);
//    indexBuffer->setData(indices.data(), sizeof(uint16_t) * 3);

    while(window->open() && engine->process())
    {
//        device->draw(program, vertexBuffer, indexBuffer);
//        window->swapBuffer();
    }
}
