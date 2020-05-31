#pragma once

#include <sigma/Texture.hpp>

#include <cstdint>

class Texture2DGL : public Texture2D {
public:
	Texture2DGL();

	virtual ~Texture2DGL();

	unsigned int handle() const { return mHandle; }

	bool initialize(const TextureCreateParams& inParams);

private:
	unsigned int mHandle = 0;
};