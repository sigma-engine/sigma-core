#include <sigma/OpenGL/DescriptorSetGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/UniformBufferGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

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

    for (const auto& [binding, buffer] : inParams.uniformBuffers) {
        SIGMA_ASSERT(std::dynamic_pointer_cast<UniformBufferGL>(buffer), "Must use opengl uniform buffer with opengl descriptor set!");
        mUniformBuffers[binding] = std::static_pointer_cast<UniformBufferGL>(buffer);
    }

    return true;
}

void DescriptorSetGL::bind()
{
    for (const auto& [binding, buffer] : mUniformBuffers) {
        CHECK_GL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer->handle()));
    }
}