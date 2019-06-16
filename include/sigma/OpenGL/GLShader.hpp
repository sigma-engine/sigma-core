#pragma once

#include <sigma/Shader.hpp>

#include <cstdint>
#include <string>

class GLShader : public Shader
{
public:
    GLShader(ShaderType inType);

	virtual ~GLShader();

    uint32_t handle() const;

    bool compile(const std::string &inCode);
private:
    uint32_t mHandle = 0;
};
