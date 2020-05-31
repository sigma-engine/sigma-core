#include <sigma/OpenGL/TextureGL.hpp>

#include <sigma/OpenGL/UtilGL.hpp>

Texture2DGL::Texture2DGL()
{
	CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &mHandle));
}

Texture2DGL::~Texture2DGL()
{
	CHECK_GL(glDeleteTextures(1, &mHandle));
}

bool Texture2DGL::initialize(const TextureCreateParams &inParams)
{
	// TODO texture types other than 2D
	SIGMA_ASSERT(inParams.size.x > 0 && inParams.size.y > 0 && inParams.size.z == 1, "Can only create 2d textures for now!");

	CHECK_GL(glBindTexture(GL_TEXTURE_2D, mHandle));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CHECK_GL(glTexImage2D(GL_TEXTURE_2D,
						  0,
						  convertImageFormatInternalGL(inParams.format),
						  inParams.size.x,
						  inParams.size.y,
						  0,
						  convertImageFormatFormatGL(inParams.format),
						  convertImageFormatTypeGL(inParams.format),
						  inParams.pixels));
	return true;
}
