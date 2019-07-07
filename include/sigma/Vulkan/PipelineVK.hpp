#pragma once

#include <sigma/Pipeline.hpp>
#include <sigma/Rect.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class RenderPassVK;
class DescriptorSetLayoutVK;

class PipelineVK : public Pipeline {
public:
    PipelineVK(std::shared_ptr<DeviceVK> inDevice);

    virtual ~PipelineVK();

    VkPipeline handle() const { return mPipeline; }

	VkPipelineLayout layout() const { return mLayout; }

    bool initialize(const PipelineCreateParams& inParams);

public:
    std::shared_ptr<DeviceVK> mDevice = nullptr;

    std::shared_ptr<RenderPassVK> mRenderPass = nullptr;
	std::vector<std::shared_ptr<DescriptorSetLayoutVK>> mSetLayouts;
    VkPipeline mPipeline = nullptr;
    VkPipelineLayout mLayout = nullptr;
};
