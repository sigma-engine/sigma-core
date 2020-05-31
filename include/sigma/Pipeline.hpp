#pragma once

#include <sigma/Rect.hpp>
#include <sigma/VertexLayout.hpp>

#include <memory>
#include <vector>

class Shader;
class RenderPass;
class DescriptorSetLayout;

struct PipelineCreateParams {
	Rect<uint32_t> viewportRect;
	std::shared_ptr<RenderPass> renderPass;
	VertexLayout vertexLayout;
	std::vector<std::shared_ptr<DescriptorSetLayout>> setLayouts;
	std::vector<std::shared_ptr<Shader>> shaders;
};

class Pipeline {
public:
	virtual ~Pipeline() = default;
};
