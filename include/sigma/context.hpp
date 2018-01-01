#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/type_sequence.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <tuple>

namespace sigma {
template <class... Resources>
using resource_set = type_set_t<Resources...>;

template <class ResourceSet>
class context;

template <class... Resources>
class context<type_set<Resources...>> {
public:
    using resource_set_type = resource_set<Resources...>;

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
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

private:
    context(const context<resource_set_type>&) = delete;
    context<resource_set_type>& operator=(const context<resource_set_type>&) = delete;

    boost::filesystem::path cache_path_;
    std::tuple<resource::cache<Resources>...> caches_;

    template <class ViewResourceSet>
    friend class context_view;
};

template<class ViewResourceSet>
class context_view;

template <class... ViewResources>
class context_view<type_set<ViewResources...>> {
public:
    using resource_set_type = resource_set<ViewResources...>;

    template <class... Resources>
    context_view(context<type_set<Resources...>>& ctx)
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
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

private:
    boost::filesystem::path& cache_path_;
    std::tuple<resource::cache<ViewResources>&...> caches_;
};
}

#endif // SIGMA_CONTEXT_HPP
