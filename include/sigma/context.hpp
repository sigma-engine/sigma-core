#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/resource/cache.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/type_sequence.hpp>

#include <boost/hana/define_struct.hpp>

#include <tuple>
#include <vector>
#include <filesystem>

namespace sigma {
template <class... Resources>
using resource_set = type_set_t<Resources...>;

template <class... Settings>
using settings_set = type_set_t<Settings...>;

template <class ResourceSet, class SettingsSet>
class context;

template <class... Resources, class... Settings>
class context<type_set<Resources...>, type_set<Settings...>> {
public:
    using resource_set_type = resource_set<Resources...>;
    using settings_set_type = settings_set<Settings...>;

    context(const std::filesystem::path& cache_path)
        : cache_path_{ cache_path }
        , caches_{ resource::cache<Resources>{ cache_path_ / resource_shortname(Resources) }... }
    {
    }

    const std::filesystem::path& get_cache_path() const
    {
        return cache_path_;
    }

    template <class U>
    inline resource::cache<U>& get_cache()
    {
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

    template <class U>
    inline const resource::cache<U>& get_cache() const
    {
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

    template <class U>
    inline U& get_settings()
    {
        return std::get<index_of_type_v<U, settings_set_type>>(settings_);
    }

    template <class U>
    inline const U& get_settings() const
    {
        return std::get<index_of_type_v<U, settings_set_type>>(settings_);
    }

private:
    context(const context<resource_set_type, settings_set_type>&) = delete;
    context<resource_set_type, settings_set_type>& operator=(const context<resource_set_type, settings_set_type>&) = delete;

    std::filesystem::path cache_path_;
    std::tuple<resource::cache<Resources>...> caches_;
    std::tuple<Settings...> settings_;

    template <class ViewResourceSet, class ViewSettingsSet>
    friend class context_view;
};

template <class ViewResourceSet, class ViewSettingsSet>
class context_view;

template <class... ViewResources, class... ViewSettings>
class context_view<type_set<ViewResources...>, type_set<ViewSettings...>> {
public:
    using resource_set_type = resource_set<ViewResources...>;
    using settings_set_type = settings_set<ViewSettings...>;

    template <class... Resources, class... Settings>
    context_view(context<type_set<Resources...>, type_set<Settings...>>& ctx)
        : cache_path_{ ctx.cache_path_ }
        , caches_{ ctx.template get_cache<ViewResources>()... }
        , settings_{ ctx.template get_settings<ViewSettings>()... }
    {
    }

    const std::filesystem::path& get_cache_path() const
    {
        return cache_path_;
    }

    template <class U>
    inline resource::cache<U>& get_cache()
    {
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

    template <class U>
    inline const resource::cache<U>& get_cache() const
    {
        return std::get<index_of_type_v<U, resource_set_type>>(caches_);
    }

    template <class U>
    inline U& get_settings()
    {
        return std::get<index_of_type_v<U, settings_set_type>>(settings_);
    }

    template <class U>
    inline const U& get_settings() const
    {
        return std::get<index_of_type_v<U, settings_set_type>>(settings_);
    }

private:
    std::filesystem::path& cache_path_;
    std::tuple<resource::cache<ViewResources>&...> caches_;
    std::tuple<ViewSettings&...> settings_;
};
}

#endif // SIGMA_CONTEXT_HPP
