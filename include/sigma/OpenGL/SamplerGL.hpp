#pragma once

#include <sigma/Sampler.hpp>

#include <glad/glad.h>

#include <cstdint>

class Sampler2DGL : public Sampler2D
{
public:
	Sampler2DGL();

	~Sampler2DGL();

	bool initialize(const SamplerCreateParams &inParams);

	GLuint handle() const { return mHandle; }

private:
	GLuint mHandle = 0;
};