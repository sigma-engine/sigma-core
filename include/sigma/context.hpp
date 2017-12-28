#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/variadic.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <tuple>

namespace sigma {

template <class... Resources>
class context {
public:
    context(const boost::filesystem::path& cache_path)
        : cache_path_{ cache_path }
        , caches_{ resource::cache<Resources>{ cache_path_ / resource_shortname(Resources) }... }
    {
    }

    const boost::filesystem::path& get_cache_path() const
    {
        return cache_path_;
    }

    template <class U>
    inline resource::cache<U>& get_cache()
    {
        return std::get<index<U, Resources...>::value>(caches_);
    }

private:
    context(const context<Resources...>&) = delete;
    context<Resources...>& operator=(const context<Resources...>&) = delete;

    boost::filesystem::path cache_path_;
    std::tuple<resource::cache<Resources>...> caches_;

    template <class... ViewResources>
    friend class context_view;
};

template <class... ViewResources>
class context_view {
public:
    template <class... Resources>
    context_view(context<Resources...>& ctx)
        : cache_path_{ ctx.cache_path_ }
        , caches_{ ctx.template get_cache<ViewResources>()... }
    {
    }

    const boost::filesystem::path& get_cache_path() const
    {
        return cache_path_;
    }

    template <class U>
    inline resource::cache<U>& get_cache()
    {
        return std::get<index<U, ViewResources...>::value>(caches_);
    }

private:
    boost::filesystem::path& cache_path_;
    std::tuple<resource::cache<ViewResources>&...> caches_;
};
}

#endif // SIGMA_CONTEXT_HPP
