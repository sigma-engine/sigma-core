#pragma once

#include <sigma/Resource/AssetCache.hpp>

#include <memory>
#include <typeindex>
#include <filesystem>
#include <unordered_map>

class Context;
class AssetLoader;
class AssetCacheBase;

class AssetManager : public std::enable_shared_from_this<AssetManager>
{
public:
    AssetManager(std::weak_ptr<Context> inContext);

    void addLoader(std::shared_ptr<AssetLoader> inLoader);

    template <class U>
    std::shared_ptr<U> getAsset(const typename U::ResourceID &inRID)
    {
        return aquireCache<U>()->get(inRID);
    }

    template <class U>
    std::shared_ptr<U> aquireAsset(const typename U::ResourceID &inRID)
    {
        std::shared_ptr<U> result = getAsset<U>(inRID);

        if (result == nullptr)
        {
            loadAsset(inRID);
            result = getAsset<U>(inRID);
        }

        return result;
    }

    template <class U>
    std::shared_ptr<AssetCache<U>> aquireCache()
    {
        std::shared_ptr<AssetCache<U>> cache = nullptr;
        auto it = mCaches.find(typeid(U));
        if (it != mCaches.end())
            cache = std::static_pointer_cast<AssetCache<U>>(it->second);
        else
            cache = std::make_shared<AssetCache<U>>();

        mCaches[typeid(U)] = cache;

        return cache;
    }

private:
    std::weak_ptr<Context> mContext;
    std::vector<std::shared_ptr<AssetLoader>> mLoaders;
    std::unordered_map<std::type_index, std::shared_ptr<AssetCacheBase>> mCaches;

    std::shared_ptr<AssetLoader> loaderFor(const std::filesystem::path &inPath);

    void loadAsset(const std::filesystem::path &inPath);
};
