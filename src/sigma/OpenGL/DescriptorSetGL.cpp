#include <sigma/OpenGL/DescriptorSetGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/UniformBufferGL.hpp>

DescriptorSetLayoutGL::DescriptorSetLayoutGL(const std::vector<DescriptorSetLayoutBinding>& inBindings)
    : mBindings(inBindings)
{
}

std::shared_ptr<DescriptorSetLayout> DescriptorSetGL::layout() const
{
	return mLayout;
}

bool DescriptorSetGL::initialize(const DescriptorSetCreateParams& inParams)
{
    SIGMA_ASSERT(std::dynamic_pointer_cast<DescriptorSetLayoutGL>(inParams.layout), "Must use opengl descriptor set layout with opengl descriptor set!");
    mLayout = std::static_pointer_cast<DescriptorSetLayoutGL>(inParams.layout);

    mUniformBuffers.resize(inParams.uniformBuffers.size());
    for (uint32_t i = 0; i < inParams.uniformBuffers.size(); ++i) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<UniformBufferGL>(inParams.uniformBuffers[i]), "Must use opengl uniform buffer with opengl descriptor set!");
        mUniformBuffers[i] = std::static_pointer_cast<UniformBufferGL>(inParams.uniformBuffers[i]);
    }

    return true;
}

void DescriptorSetGL::bind()
{
	for (uint32_t i = 0; i < mUniformBuffers.size(); ++i)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, i, mUniformBuffers[i]->handle());
	}
}