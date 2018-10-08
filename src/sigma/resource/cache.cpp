#include <sigma/resource/cache.hpp>

#include <sigma/context.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/serialization/unordered_map.hpp>

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
        , cache_path_(context_->cache_path() / "data" / short_name)
    {
        if (!boost::filesystem::exists(cache_path_))
            boost::filesystem::create_directories(cache_path_);
    }

    bool base_cache::exists(const key_type& key) const
    {
        return boost::filesystem::exists(cache_path_ / key);
    }

    std::time_t base_cache::last_modification_time(const key_type& key) const
    {
        auto path = cache_path_ / key;
        if (boost::filesystem::exists(path))
            return boost::filesystem::last_write_time(path);
        return 0;
    }
}
}
