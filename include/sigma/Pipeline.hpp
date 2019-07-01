#pragma once

#include <sigma/Rect.hpp>

#include <memory>
#include <vector>

class Shader;
class RenderPass;

struct PipelineCreateParams
{
    Rect<uint32_t> viewportRect;
    std::shared_ptr<RenderPass> renderPass;
    std::vector<std::shared_ptr<Shader>> shaders;
};

class Pipeline {
public:
    virtual ~Pipeline() = default;
};
