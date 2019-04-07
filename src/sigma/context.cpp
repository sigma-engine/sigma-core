#include <sigma/tools/json_conversion.hpp>
#include <sigma/context.hpp>
#include <sigma/resource/cache.hpp>

namespace sigma {

context::context(const std::filesystem::path& cache_path)
    : cache_path_ { cache_path }
{
}

const std::filesystem::path& context::cache_path() const
{
    return cache_path_;
}

void context::write_database()
{
    for(auto [type, cache] : caches_)
    {
        cache->save();
    }
}
}


