#pragma once

#include <cstdint>

#include <sigma/FlagSet.hpp>

#include <glm/vec3.hpp>

#include <string>

enum class ImageFormat {
	Unknown,
	UnormB8G8R8A8,
	UnormR8G8B8A8
};

enum class ImageUsage {
	Sampler = 1 << 0, // The Image will be sampled in a Shader.
	ColorAttachment = 1 << 1, // The Image will be used as a color attachment in a FrameBuffer.
	DepthStencilAttachment = 1 << 2 // The Image will be used as a depth or stencil attachment in a FrameBuffer.
};

FlagSet<ImageUsage> operator|(const ImageUsage& inA, const ImageUsage& inB);

uint64_t imageFormatPixelSize(ImageFormat inFormat);

struct TextureCreateParams {
	glm::uvec3 size;
	ImageFormat format;
	FlagSet<ImageUsage> usage = ImageUsage::Sampler;
	void* pixels = nullptr;
};

class Texture2D {
public:
	using ResourceID = std::string;
	virtual ~Texture2D() = default;
};