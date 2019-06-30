#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>

class Surface;

enum class DataType;

class Shader;
enum class ShaderType;

class Program;

class VertexBuffer;
struct VertexMemberDescription;

class IndexBuffer;
enum class PrimitiveType;

enum class DeviceType
{
    DiscreteGPU,
    IntegratedGPU,
    Unknown
};

struct DeviceUsageParams
{
    uint32_t graphicsQueueCount;
    uint32_t computeQueueCount;
    std::shared_ptr<Surface> presentSurface;
};

class Device {
public:
    virtual ~Device() = default;

    virtual DeviceType type() const = 0;

    virtual uint32_t maxGraphicsQueues() const = 0;

    virtual uint32_t maxComputeQueues() const = 0;

    virtual bool supportsSurface(std::shared_ptr<Surface> inSurface) const = 0;

    virtual bool initialize(const std::vector<std::shared_ptr<Surface>>& inSurfaces) = 0;

    virtual std::shared_ptr<Shader> createShader(ShaderType inType, const std::string& inCode) = 0;

    virtual std::shared_ptr<Program> createProgram(const std::vector<std::shared_ptr<Shader>>& inShaders) = 0;

    virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const std::initializer_list<VertexMemberDescription>& inLayout) = 0;

    virtual std::shared_ptr<IndexBuffer> createIndexBuffer(PrimitiveType inPrimitive, DataType inDataType) = 0;

    virtual void draw(std::shared_ptr<Program> inProgram, std::shared_ptr<VertexBuffer> inVertexBuffer, std::shared_ptr<IndexBuffer> inIndexBuffer) = 0;
};
