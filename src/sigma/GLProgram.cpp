#include <sigma/GLProgram.hpp>

#include <sigma/GLShader.hpp>
#include <sigma/Log.hpp>

#include <glad/glad.h>

#include <cassert>
#include <vector>

GLProgram::GLProgram()
{
    mHandle = glCreateProgram();
}

GLProgram::~GLProgram()
{
    glDeleteProgram(mHandle);
}

void GLProgram::attach(std::shared_ptr<Shader> inShader)
{
    assert(std::dynamic_pointer_cast<GLShader>(inShader));
    auto glShader = std::static_pointer_cast<GLShader>(inShader);
    glAttachShader(mHandle, glShader->handle());
}

bool GLProgram::link()
{
    GLint linked = GL_FALSE;
    glLinkProgram(mHandle);
    glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        GLint length = 0;
        glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> errorBuffer(static_cast<std::size_t>(length));
        glGetProgramInfoLog(mHandle, length, &length, errorBuffer.data());
        SIGMA_ERROR("{}", errorBuffer.data());
        return false;
    }

    return true;
}

void GLProgram::bind() const
{
    glUseProgram(mHandle);
}
