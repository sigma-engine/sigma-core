#pragma once

#include <sigma/Algorithm.hpp>

enum class SamplerWarpMode
{
	ClampToEdge,
	MirroredRepeat,
	Repeat
};

enum class SamplerFilterMode
{
	Nearest,
	Linear,
	Cubic
};

enum class SamplerMipmapMode
{
	None,
	Nearest,
	Linear
};

struct SamplerCreateParams
{
	SamplerMipmapMode mipmapMode = SamplerMipmapMode::Linear;
	SamplerFilterMode minFilter = SamplerFilterMode::Linear;
	SamplerFilterMode magFilter = SamplerFilterMode::Linear;
	SamplerWarpMode warpU = SamplerWarpMode::Repeat;
	SamplerWarpMode warpV = SamplerWarpMode::Repeat;
	SamplerWarpMode warpW = SamplerWarpMode::Repeat;
};

class Sampler2D
{
public:
	using ResourceID = SamplerCreateParams;

	virtual ~Sampler2D() = default;
};

namespace std {
	template<>
	struct hash<SamplerCreateParams>
	{
		size_t operator()(const SamplerCreateParams &inValue) const
		{
			return HashCombine(
				inValue.mipmapMode,
				inValue.minFilter,
				inValue.magFilter,
				inValue.warpU,
				inValue.warpV, 
				inValue.warpW
			);
		}
	};
}