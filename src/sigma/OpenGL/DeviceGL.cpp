#include <sigma/OpenGL/DeviceGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/IndexBufferGL.hpp>
#include <sigma/OpenGL/ProgramGL.hpp>
#include <sigma/OpenGL/ShaderGL.hpp>
#include <sigma/OpenGL/VertexBufferGL.hpp>

#include <glad/glad.h>

#include <algorithm>
#include <cassert>
#include <iostream>

void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        SIGMA_ERROR("0x{:x}: {}", id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        SIGMA_ERROR("0x{:x}: {}", id, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        SIGMA_CRITICAL("0x{:x}: {}", id, message);
        break;
    default:
        return;
    }
}

bool DeviceGL::initialize(const std::set<std::string> &inRequiredExtensions)
{
    gladLoadGL();

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, true);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false);

    GLint extensionCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
    for (GLint i = 0; i < extensionCount; ++i) {
        const char* ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
        mExtensions.insert(ext);
    }

    SIGMA_INFO("Vendor: {}", glGetString(GL_VENDOR));
    SIGMA_INFO("Model: {}", glGetString(GL_RENDERER));
    SIGMA_INFO("Version: {}", glGetString(GL_VERSION));
    // TODO: Get total video memory http://nasutechtips.blogspot.com/2011/02/how-to-get-gpu-memory-size-and-usage-in.html

    for(auto reqExt: inRequiredExtensions)
    {
        if(!mExtensions.count(reqExt))
        {
            SIGMA_ERROR("Missing Required OpenGL Extension: {}", reqExt);
            SIGMA_INFO("Supported OpenGL Extensions: {}", fmt::join(mExtensions.begin(), mExtensions.end(), ","));
            return false;
        }
    }

    return true;
}

std::shared_ptr<Shader> DeviceGL::createShader(ShaderType inType, const std::string& inCode)
{
    auto shader = std::make_shared<ShaderGL>(inType);
    if (shader->compile(inCode)) {
        return std::move(shader);
    }
    return nullptr;
}

std::shared_ptr<Program> DeviceGL::createProgram(const std::vector<std::shared_ptr<Shader>>& inShaders)
{
    auto program = std::make_shared<ProgramGL>();
    std::for_each(inShaders.begin(), inShaders.end(), [&](auto s) { program->attach(s); });
    if (program->link()) {
        return std::move(program);
    }

    return nullptr;
}

std::shared_ptr<VertexBuffer> DeviceGL::createVertexBuffer(const std::initializer_list<VertexMemberDescription>& inLayout)
{
    return std::make_shared<VertexBufferGL>(inLayout);
}

std::shared_ptr<IndexBuffer> DeviceGL::createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType)
{
    assert((inDataType == DataType::UInt || inDataType == DataType::UShort) && "Invlaid data type");
    return std::make_shared<IndexBufferGL>(inPrimitive, inDataType);
}

void DeviceGL::draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer)
{
    assert(std::dynamic_pointer_cast<ProgramGL>(inProgram) && std::dynamic_pointer_cast<VertexBufferGL>(inVertexBuffer) && std::dynamic_pointer_cast<IndexBufferGL>(inIndexBuffer) && "Must use OpenGL programs, vertex buffers, and index buffers.");
    auto program = std::static_pointer_cast<ProgramGL>(inProgram);
    auto vertexBuffer = std::static_pointer_cast<VertexBufferGL>(inVertexBuffer);
    auto indexBuffer = std::static_pointer_cast<IndexBufferGL>(inIndexBuffer);

    program->bind();
    vertexBuffer->bind();
    indexBuffer->draw();
}
