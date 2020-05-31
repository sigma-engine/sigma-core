#include <sigma/OpenGL/SamplerGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

Sampler2DGL::Sampler2DGL()
{
	CHECK_GL(glGenSamplers(1, &mHandle));
}

Sampler2DGL::~Sampler2DGL()
{
	CHECK_GL(glDeleteSamplers(1, &mHandle));
}

bool Sampler2DGL::initialize(const SamplerCreateParams &inParams)
{
	CHECK_GL(glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_S, convertSamplerWarpModeGL(inParams.warpU)));
	CHECK_GL(glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_T, convertSamplerWarpModeGL(inParams.warpV)));
	CHECK_GL(glSamplerParameteri(mHandle, GL_TEXTURE_WRAP_R, convertSamplerWarpModeGL(inParams.warpW)));
	CHECK_GL(glSamplerParameteri(mHandle, GL_TEXTURE_MIN_FILTER, convertSamplerFilterModeGL(inParams.minFilter, inParams.mipmapMode)));
	CHECK_GL(glSamplerParameteri(mHandle, GL_TEXTURE_MAG_FILTER, convertSamplerFilterModeGL(inParams.minFilter, SamplerMipmapMode::None)));
	return true;
}