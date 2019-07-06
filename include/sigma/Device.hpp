#pragma once

#include <sigma/Rect.hpp>

#include <memory>
#include <set>
#include <string>
#include <vector>

class CommandBuffer;

class Surface;

enum class DataType;

class Shader;
enum class ShaderType;

struct RenderPassCreateParams;
class RenderPass;

struct PipelineCreateParams;
class Pipeline;

class VertexBuffer;
struct VertexMemberDescription;

class IndexBuffer;
enum class PrimitiveType;

class VertexLayout;

enum class DeviceType {
    DiscreteGPU,
    IntegratedGPU,
    Unknown
};

class Device {
public:
    virtual ~Device() = default;

    virtual DeviceType type() const = 0;

    virtual uint32_t maxGraphicsQueues() const = 0;

    virtual uint32_t maxComputeQueues() const = 0;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const = 0;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces) = 0;

    virtual std::shared_ptr<CommandBuffer> createCommandBuffer() = 0;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string& inSourcePath) = 0;

    virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPassCreateParams& inParams) = 0;

    virtual std::shared_ptr<Pipeline> createPipeline(const PipelineCreateParams& inParams) = 0;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexLayout& inLayout, uint64_t inSize) = 0;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType, uint64_t inSize) = 0;
};
