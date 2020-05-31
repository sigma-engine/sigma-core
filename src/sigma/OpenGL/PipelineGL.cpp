#include <sigma/OpenGL/PipelineGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/ShaderGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

#include <cassert>
#include <vector>

PipelineGL::PipelineGL()
{
	CHECK_GL(mHandle = glCreateProgram());
}

PipelineGL::~PipelineGL()
{
	CHECK_GL(glDeleteProgram(mHandle));
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
	SIGMA_ASSERT(std::dynamic_pointer_cast<ShaderGL>(inShader), "Must use opengl shader with opengl pipeline!");
	auto shader = std::static_pointer_cast<ShaderGL>(inShader);
	glAttachShader(mHandle, shader->handle());
}

bool PipelineGL::link()
{
	GLint linked = GL_FALSE;
	CHECK_GL(glLinkProgram(mHandle));
	CHECK_GL(glGetProgramiv(mHandle, GL_LINK_STATUS, &linked));
	if (linked == GL_FALSE) {
		GLint length = 0;
		CHECK_GL(glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length));
		std::vector<GLchar> errorBuffer(static_cast<std::size_t>(length));
		CHECK_GL(glGetProgramInfoLog(mHandle, length, &length, errorBuffer.data()));
		SIGMA_ERROR("{}", errorBuffer.data());
		return false;
	}

	return true;
}

void PipelineGL::bind() const
{
	CHECK_GL(glUseProgram(mHandle));
}
