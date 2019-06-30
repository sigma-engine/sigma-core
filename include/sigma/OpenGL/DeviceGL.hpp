#pragma once

#include <sigma/Device.hpp>

class DeviceGL : public Device {
public:
    virtual DeviceType type() const override;

    virtual uint32_t maxGraphicsQueues() const override;

    virtual uint32_t maxComputeQueues() const override;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const override;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface> > &inSurfaces) override;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string& inCode) override;

    virtual std::shared_ptr<Program> createProgram(const std::vector<std::shared_ptr<Shader>>& inShaders) override;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const std::initializer_list<VertexMemberDescription>& inLayout) override;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType) override;

    virtual void draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer) override;
};
