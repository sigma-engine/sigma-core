#pragma once

#include <sigma/Device.hpp>

class DeviceGL : public Device, public std::enable_shared_from_this<DeviceGL> {
public:
    virtual DeviceType type() const override;

    virtual uint32_t maxGraphicsQueues() const override;

    virtual uint32_t maxComputeQueues() const override;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const override;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces) override;

    virtual std::shared_ptr<CommandBuffer> createCommandBuffer() override;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string& inSourcePath) override;

    virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPassCreateParams& inParams) override;

    virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const std::vector<DescriptorSetLayoutBinding>& inBindings) override;

	virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSetCreateParams &inParams) override;

    virtual std::shared_ptr<Pipeline> createPipeline(const PipelineCreateParams& inParams) override;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexLayout& inLayout, uint64_t inSize) override;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize) override;

    virtual std::shared_ptr<UniformBuffer> createUniformBuffer(uint64_t inSize) override;
};
