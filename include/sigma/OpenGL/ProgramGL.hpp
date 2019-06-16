#pragma once

#include <sigma/Program.hpp>

#include <cstdint>
#include <memory>

class Shader;

class ProgramGL : public Program {
public:
    ProgramGL();

    virtual ~ProgramGL();

    void attach(std::shared_ptr<Shader> inShader);

    bool link();

    void bind() const;

private:
    uint32_t mHandle = 0;
};
