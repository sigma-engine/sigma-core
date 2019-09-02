#pragma once

#include <memory>

class Engine;
class Device;
class AssetManager;

class Context : public std::enable_shared_from_this<Context>
{
public:
	bool initialize(std::shared_ptr<Engine> inEngine, std::shared_ptr<Device> inDevice);

    std::shared_ptr<Engine> engine() const { return mEngine; }

    std::shared_ptr<Device> device() const { return mDevice; }

    std::shared_ptr<AssetManager> assetManager() { return mAssetManager; }

private:
    std::shared_ptr<Engine> mEngine = nullptr;
    std::shared_ptr<Device> mDevice = nullptr;
    std::shared_ptr<AssetManager> mAssetManager = nullptr;
};