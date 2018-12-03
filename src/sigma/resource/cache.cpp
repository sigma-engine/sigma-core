#include <sigma/resource/cache.hpp>

#include <sigma/context.hpp>

#include <filesystem>

namespace sigma {
namespace resource {

    missing_resource::missing_resource(const key_type& key)
    {
        message_ = "missing resource " + key.string();
    }

    const char* missing_resource::what() const noexcept
    {
        return message_.c_str();
    }

    base_cache::base_cache(std::shared_ptr<context> context, const std::string& short_name)
        : context_(context)
        , cache_path_(context->cache_path() / "data" / short_name)
    {
        if (!std::filesystem::exists(cache_path_)) {
            std::filesystem::create_directories(cache_path_);
        }
    }

    bool base_cache::exists(const key_type& key) const
    {
        return std::filesystem::exists(cache_path_ / key);
    }
}
}
