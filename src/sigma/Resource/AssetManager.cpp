#include <sigma/Resource/AssetManager.hpp>

#include <sigma/Resource/AssetLoader.hpp>

AssetManager::AssetManager(std::weak_ptr<Context> inContext)
    : mContext(inContext)
{
}

void AssetManager::addLoader(std::shared_ptr<AssetLoader> inLoader)
{
    mLoaders.push_back(inLoader);
}

std::shared_ptr<AssetLoader> AssetManager::loaderFor(const std::filesystem::path &inPath)
{
    for (auto loader : mLoaders)
    {
        if (loader->supports(inPath))
            return loader;
    }
    return nullptr;
}

void AssetManager::loadAsset(const std::filesystem::path &inPath)
{
    auto loader = loaderFor(inPath);
    loader->load(mContext.lock(), inPath);
}