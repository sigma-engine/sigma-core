#include <sigma/Engine.hpp>
#include <sigma/Window.hpp>
#include <sigma/Device.hpp>
#include <sigma/Shader.hpp>
#include <sigma/VertexBuffer.hpp>
#include <sigma/IndexBuffer.hpp>
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

    auto window = engine->createWindow("Simple Game", 1920 / 2, 1080 / 2);
    if (window == nullptr)
        return -1;
    
    auto device = engine->graphicsDevice();
    if (device == nullptr)
        return -1;

//    auto vertexShader = device->createShader(ShaderType::VertexShader, R"(
//        #version 450
//        layout(location = 0) in vec3 position;
//        layout(location = 1) in vec3 color;
//        layout(location = 0) uniform sampler2D texture;

//        out vec3 vColor;

//        void main()
//        {
//            vColor = color;
//            gl_Position = vec4(position, 1);
//        }
//    )");
    
//    auto fragmentShader = device->createShader(ShaderType::FragmentShader, R"(
//        #version 450

//        layout(location = 0) out vec4 color;
//        layout(location = 0) uniform sampler2D texture;

//        in vec3 vColor;

//        void main()
//        {
//            color = vec4(vColor, 1);
//        }
//    )");
    
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
