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
