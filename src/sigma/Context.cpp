#include <sigma/Context.hpp>

#include <sigma/Resource/AssetManager.hpp>


bool Context::initialize(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice)
{
	mEngine = inEngine;
	mDevice = inDevice;
	mAssetManager = std::make_shared<AssetManager>(shared_from_this());

	return true;
}