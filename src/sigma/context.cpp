#include <sigma/context.hpp>
#include <sigma/graphics/pipeline.hpp>

namespace sigma {

context::context(const boost::filesystem::path& cache_path)
    : cache_path_ { cache_path }
    , pipeline_ { std::make_shared<graphics::pipeline>() }
{
}

const boost::filesystem::path& context::cache_path() const
{
    return cache_path_;
}

std::shared_ptr<graphics::pipeline> context::pipeline()
{
    return pipeline_;
}
}
