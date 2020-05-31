#pragma once

#include <filesystem>

class Context;

class AssetLoader
{
public:
	virtual ~AssetLoader() = default;

	virtual bool supports(const std::filesystem::path &inPath) = 0;

	virtual void load(std::shared_ptr<Context> inContext, const std::filesystem::path &inPath) = 0;
};