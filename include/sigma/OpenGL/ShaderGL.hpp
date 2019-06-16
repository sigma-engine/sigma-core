#pragma once

#include <sigma/Shader.hpp>

#include <cstdint>
#include <string>

class ShaderGL : public Shader {
public:
    ShaderGL(ShaderType inType);

    virtual ~ShaderGL();

    uint32_t handle() const;

    bool compile(const std::string& inCode);

private:
    uint32_t mHandle = 0;
};
