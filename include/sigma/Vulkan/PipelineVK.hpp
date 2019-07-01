#pragma once

#include <sigma/Pipeline.hpp>
#include <sigma/Rect.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class RenderPassVK;

class PipelineVK : public Pipeline {
public:
    PipelineVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~PipelineVK();

    bool initialize(const PipelineCreateParams &inParams);

public:
    std::shared_ptr<DeviceVK> mDevice = nullptr;

    std::shared_ptr<RenderPassVK> mRenderPass = nullptr;
    VkPipeline mPipeline = nullptr;
    VkPipelineLayout mLayout = nullptr;
};
