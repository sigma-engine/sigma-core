#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

class AssetCacheBase
{
public:
    virtual ~AssetCacheBase() = default;

private:
};

template <class T>
class AssetCache : public AssetCacheBase
{
public:
    std::shared_ptr<T> get(const std::filesystem::path &inPath)
    {
        auto it = mMap.find(inPath.string());
        if (it != mMap.end())
            return it->second;
        return nullptr;
    }

    void insert(const std::filesystem::path &inPath, std::shared_ptr<T> inItem)
    {
        mMap[inPath.string()] = inItem;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> mMap;
};
