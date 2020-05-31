#include <sigma/OpenGL/DescriptorSetGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/SamplerGL.hpp>
#include <sigma/OpenGL/TextureGL.hpp>
#include <sigma/OpenGL/UniformBufferGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

DescriptorSetLayoutGL::DescriptorSetLayoutGL(const std::vector<DescriptorSetLayoutBinding> &inBindings)
	: mBindings(inBindings)
{
}

std::shared_ptr<DescriptorSetLayout> DescriptorSetGL::layout() const
{
	return mLayout;
}

bool DescriptorSetGL::initialize(const DescriptorSetCreateParams &inParams)
{
	SIGMA_ASSERT(std::dynamic_pointer_cast<DescriptorSetLayoutGL>(inParams.layout), "Must use opengl descriptor set layout with opengl descriptor set!");
	mLayout = std::static_pointer_cast<DescriptorSetLayoutGL>(inParams.layout);

	for (const auto &[binding, buffer] : inParams.uniformBuffers)
	{
		SIGMA_ASSERT(std::dynamic_pointer_cast<UniformBufferGL>(buffer), "Must use opengl uniform buffer with opengl descriptor set!");
		mUniformBuffers[binding] = std::static_pointer_cast<UniformBufferGL>(buffer);
	}

	for (const auto &[binding, combindedSampler] : inParams.imageSamplers)
	{
		SIGMA_ASSERT(std::dynamic_pointer_cast<Texture2DGL>(combindedSampler.image), "Must use opengl texture with opengl descriptor set!");
		SIGMA_ASSERT(std::dynamic_pointer_cast<Sampler2DGL>(combindedSampler.sampler), "Must use opengl sampler with opengl descriptor set!");
		mImageSamplers[binding].image = std::static_pointer_cast<Texture2DGL>(combindedSampler.image);
		mImageSamplers[binding].sampler = std::static_pointer_cast<Sampler2DGL>(combindedSampler.sampler);
	}

	return true;
}

void DescriptorSetGL::bind()
{
	for (const auto &[binding, buffer] : mUniformBuffers)
	{
		CHECK_GL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer->handle()));
	}

	for (const auto &[binding, combindedSampler] : mImageSamplers)
	{
		// TODO: texture types other than 2D
		CHECK_GL(glActiveTexture(GL_TEXTURE0 + binding));
		CHECK_GL(glBindTexture(GL_TEXTURE_2D, combindedSampler.image->handle()));
		CHECK_GL(glBindSampler(binding, combindedSampler.sampler->handle()));
	}
}