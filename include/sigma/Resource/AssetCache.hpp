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
	using ResourceID = typename T::ResourceID;

	std::shared_ptr<T> get(const ResourceID &inRID)
	{
		auto it = mMap.find(inRID);
		if (it != mMap.end())
			return it->second;
		return nullptr;
	}

	void insert(const ResourceID &inRID, std::shared_ptr<T> inItem)
	{
		mMap[inRID] = inItem;
	}

private:
	std::unordered_map<ResourceID, std::shared_ptr<T>> mMap;
};
