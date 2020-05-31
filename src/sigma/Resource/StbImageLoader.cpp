#include <sigma/Resource/StbImageLoader.hpp>

#include <sigma/Context.hpp>
#include <sigma/Texture.hpp>
#include <sigma/Resource/AssetManager.hpp>
#include <sigma/Device.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <set>
#include <algorithm>
#include <cctype>

bool StbImageLoader::supports(const std::filesystem::path &inPath)
{
	static const std::set<std::string> supported_extensions = {".jpg", ".jpeg", ".jpe"};
	std::string ext = inPath.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](std::string::value_type c) { return std::tolower(c); });

	return supported_extensions.count(ext);
}

void StbImageLoader::load(std::shared_ptr<Context> inContext, const std::filesystem::path &inPath)
{
	std::string pathStr = inPath.string();
	int width, height, planes;
	auto device = inContext->device();
	auto textureCache = inContext->assetManager()->aquireCache<Texture2D>();

	stbi_uc *pixels = stbi_load(pathStr.c_str(), &width, &height, &planes, 4);
	if (pixels)
	{
		TextureCreateParams textureParams = {
			glm::uvec3{width, height, 1},
			ImageFormat::UnormR8G8B8A8,
			ImageUsage::Sampler,
			pixels};

		textureCache->insert(inPath.string(), device->createTexture2D(textureParams));

		stbi_image_free(pixels);
	}
}