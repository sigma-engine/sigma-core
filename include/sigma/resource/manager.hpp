#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/handle.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <fstream>
#include <unordered_map>

namespace sigma {
namespace resource {

    template <class>
    using handle = sigma::handle;

    template <class Resource, class ResourceIdentifier = boost::filesystem::path>
    struct resource_loader {
        std::unique_ptr<Resource> operator()(const boost::filesystem::path& cache_directory, const ResourceIdentifier& id) const
        {
            auto resource_path = cache_directory / id;
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            auto data = std::make_unique<Resource>();
            ia >> *data;
            return data;
        }
    };

    template <class Resource, class ResourceIdentifier = boost::filesystem::path>
    class cache {
    public:
        cache(const boost::filesystem::path& cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        cache(cache<Resource, ResourceIdentifier>&&) = default;

        cache& operator=(cache<Resource, ResourceIdentifier>&&) = default;

        virtual ~cache() = default;

        virtual handle<Resource> get(const ResourceIdentifier& id)
        {
            auto it = handle_map_.find(id);
            if (it != handle_map_.end())
                return it->second;

            auto h = handle_map_[id] = create_handle(handles_, free_handles_);
            if (h.index >= resources_.size())
                resources_.resize(h.index + 1);

            resources_[h.index] = std::move(loader_(cache_directory_, id));

            return h;
        }

        Resource* acquire(handle<Resource> hnd) const
        {
            return resources_.at(hnd.index).get();
        }

    private:
        cache(const cache<Resource, ResourceIdentifier>&) = delete;
        cache& operator=(const cache<Resource, ResourceIdentifier>&) = delete;

        resource_loader<Resource, ResourceIdentifier> loader_;

        boost::filesystem::path cache_directory_;
        std::unordered_map<ResourceIdentifier, sigma::handle> handle_map_;

        std::vector<sigma::handle> handles_;
        std::vector<std::uint32_t> free_handles_;
        // TODO remove the use of unique_ptr
        std::vector<std::unique_ptr<Resource>> resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
