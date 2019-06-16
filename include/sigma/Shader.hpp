#pragma once

enum class ShaderType
{
    VertexShader,
    FragmentShader
};

class Shader
{
public:
    Shader(ShaderType inType)
        : mType(inType)
    {
    }

	virtual ~Shader() = default;

    virtual ShaderType type() const { return mType; }

public:
    ShaderType mType;
};
