#pragma once

#include <sigma/Pipeline.hpp>

#include <cstdint>
#include <memory>

class Shader;

class PipelineGL : public Pipeline {
public:
    PipelineGL();

    virtual ~PipelineGL();

    bool initialize(const PipelineCreateParams& inParams);

    void attach(std::shared_ptr<Shader> inShader);

    bool link();

    void bind() const;

private:
    uint32_t mHandle = 0;
};
