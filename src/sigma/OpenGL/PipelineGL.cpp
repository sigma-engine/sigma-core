#include <sigma/OpenGL/PipelineGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/ShaderGL.hpp>

#include <glad/glad.h>

#include <cassert>
#include <vector>

PipelineGL::PipelineGL()
{
    mHandle = glCreateProgram();
}

PipelineGL::~PipelineGL()
{
    glDeleteProgram(mHandle);
}

bool PipelineGL::initialize(const PipelineCreateParams& inParams)
{
    for (auto shader : inParams.shaders) {
        attach(shader);
    }

    return link();
}

void PipelineGL::attach(std::shared_ptr<Shader> inShader)
{
    assert(std::dynamic_pointer_cast<ShaderGL>(inShader));
    auto shader = std::static_pointer_cast<ShaderGL>(inShader);
    glAttachShader(mHandle, shader->handle());
}

bool PipelineGL::link()
{
    GLint linked = GL_FALSE;
    glLinkProgram(mHandle);
    glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> errorBuffer(static_cast<std::size_t>(length));
        glGetProgramInfoLog(mHandle, length, &length, errorBuffer.data());
        SIGMA_ERROR("{}", errorBuffer.data());
        return false;
    }

    return true;
}

void PipelineGL::bind() const
{
    glUseProgram(mHandle);
}
