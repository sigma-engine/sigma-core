#include <sigma/context.hpp>

namespace sigma {

context::context(const std::filesystem::path& cache_path)
    : cache_path_ { cache_path }
{
}

const std::filesystem::path& context::cache_path() const
{
    return cache_path_;
}
}
