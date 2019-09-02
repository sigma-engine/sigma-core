#pragma once
#include <sigma/Resource/AssetLoader.hpp>

#include <vector>
#include <string>

class StbImageLoader : public AssetLoader
{
public:
    virtual bool supports(const std::filesystem::path &inPath) override;

    virtual void load(std::shared_ptr<Context> inContext, const std::filesystem::path &inPath) override;
};