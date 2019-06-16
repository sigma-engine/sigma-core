#include <sigma/Log.hpp>
#include <sigma/OpenGL/ShaderGL.hpp>

#include <glad/glad.h>

#include <vector>

constexpr GLenum glShaderType(ShaderType inType)
{
    switch (inType) {
    case ShaderType::VertexShader:
        return GL_VERTEX_SHADER;
    case ShaderType::FragmentShader:
        return GL_FRAGMENT_SHADER;
    }
    return GL_INVALID_ENUM;
}

ShaderGL::ShaderGL(ShaderType inType)
    : Shader(inType)
{
    mHandle = glCreateShader(glShaderType(inType));
}

ShaderGL::~ShaderGL()
{
    glDeleteShader(mHandle);
}

uint32_t ShaderGL::handle() const
{
    return mHandle;
}

bool ShaderGL::compile(const std::string& inCode)
{
    const char* srcCode = inCode.c_str();
    GLint compiled;
    glShaderSource(mHandle, 1, &srcCode, nullptr);
    glCompileShader(mHandle);
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> errorBuffer(static_cast<std::size_t>(length));
        glGetShaderInfoLog(mHandle, length, &length, errorBuffer.data());
        SIGMA_ERROR("{}", errorBuffer.data());

        return false;
    }

    return true;
}
