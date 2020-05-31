#pragma once

#include <sigma/Sampler.hpp>

#include <cstdint>

class Sampler2DGL : public Sampler2D
{
public:
	Sampler2DGL();

	~Sampler2DGL();

	bool initialize(const SamplerCreateParams &inParams);

	unsigned int handle() const { return mHandle; }

private:
	unsigned int mHandle = 0;
};