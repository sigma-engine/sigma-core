#include <sigma/OpenGL/DeviceGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/CommandBufferGL.hpp>
#include <sigma/OpenGL/DescriptorSetGL.hpp>
#include <sigma/OpenGL/IndexBufferGL.hpp>
#include <sigma/OpenGL/PipelineGL.hpp>
#include <sigma/OpenGL/RenderPassGL.hpp>
#include <sigma/OpenGL/ShaderGL.hpp>
#include <sigma/OpenGL/SurfaceGL.hpp>
#include <sigma/OpenGL/VertexBufferGL.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>
#include <fstream>

DeviceType DeviceGL::type() const
{
    return DeviceType::Unknown; // TODO: there has to be a way to get the gpu type
}

uint32_t DeviceGL::maxGraphicsQueues() const
{
    return 1;
}

uint32_t DeviceGL::maxComputeQueues() const
{
    return 1; // TODO: return 0 if compute is not supported
}

bool DeviceGL::supportsSurface(std::shared_ptr<Surface> inSurface) const
{
    return true;
}

bool DeviceGL::initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces)
{
    GLint majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    SIGMA_INFO("Graphics API: OpenGL");
    SIGMA_INFO("Vendor: {}", glGetString(GL_VENDOR));
    SIGMA_INFO("Model: {}", glGetString(GL_RENDERER));
    SIGMA_INFO("API Version: {}.{}", majorVersion, minorVersion);
    SIGMA_INFO("Driver Version: {}", glGetString(GL_VERSION));
    // TODO: Get total video memory http://nasutechtips.blogspot.com/2011/02/how-to-get-gpu-memory-size-and-usage-in.html

    for (std::size_t i = 0; i < inSurfaces.size(); ++i) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<SurfaceGL>(inSurfaces[i]), "Must use opengl surfaces with opengl device!");
        auto surface = std::static_pointer_cast<SurfaceGL>(inSurfaces[i]);
        if (!surface->createSwapChain(shared_from_this())) {
            SIGMA_ERROR("Could not create render pass!");
            return false;
        }
    }
    return true;
}

std::shared_ptr<CommandBuffer> DeviceGL::createCommandBuffer()
{
    return std::make_shared<CommandBufferGL>();
}

std::shared_ptr<Shader> DeviceGL::createShader(ShaderType inType, const std::string& inSourcePath)
{
    std::ifstream file("opengl/" + inSourcePath + ".glsl", std::ios::ate | std::ios::binary);
    if (!file.is_open())
        return nullptr;
    size_t fsize = file.tellg();
    std::string code(fsize, '\n');
    file.seekg(0);
    file.read(code.data(), fsize);

    auto shader = std::make_shared<ShaderGL>(inType);
    if (shader->compile(code)) {
        return std::move(shader);
    }
    return nullptr;
}

std::shared_ptr<RenderPass> DeviceGL::createRenderPass(const RenderPassCreateParams& inParams)
{
    return std::make_shared<RenderPassGL>();
}

std::shared_ptr<DescriptorSetLayout> DeviceGL::createDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding> &inBindings)
{
    return std::make_shared<DescriptorSetLayout>();
}

std::shared_ptr<Pipeline> DeviceGL::createPipeline(const PipelineCreateParams& inParams)
{
    auto pipeline = std::make_shared<PipelineGL>();
    if (!pipeline->initialize(inParams))
        return nullptr;
    return pipeline;
}

std::shared_ptr<VertexBuffer> DeviceGL::createVertexBuffer(const VertexLayout& inLayout, uint64_t inSize)
{
    return std::make_shared<VertexBufferGL>(inLayout);
}

std::shared_ptr<IndexBuffer> DeviceGL::createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize)
{
    assert((inDataType == DataType::UInt || inDataType == DataType::UShort) && "Invlaid data type");
    return std::make_shared<IndexBufferGL>(inPrimitive, inDataType);
}