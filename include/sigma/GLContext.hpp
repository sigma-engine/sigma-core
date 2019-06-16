#pragma once

#include <sigma/Context.hpp>

class GLContext : public Context
{
public:
    virtual void initialize() override;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string &inCode) override;

    virtual std::shared_ptr<Program> createProgram(const std::vector<std::shared_ptr<Shader>> &inShaders) override;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const std::initializer_list<VertexMemberDescription> &inLayout) override;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType) override;

    virtual void draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer) override;
private:
    std::vector<std::string> mExtensions;
};
