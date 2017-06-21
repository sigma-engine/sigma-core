#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/handle.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <fstream>
#include <iostream> // TODO this is bad
#include <unordered_map>

namespace sigma {
namespace resource {

    template <class>
    using handle = sigma::handle;

    template <class Resource>
    class cache {
    public:
        // TODO remove the use of unique_ptr

        cache(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        virtual ~cache() = default;

        virtual handle<Resource> get(boost::filesystem::path id)
        {
            auto it = handle_map_.find(id);
            if (it != handle_map_.end())
                return it->second;

            auto h = handle_map_[id] = create_handle(handles_, free_handles_);
            if (h.index >= resources_.size())
                resources_.resize(h.index + 1);

            resources_[h.index] = std::move(load(id));

            return h;
        }

        Resource* acquire(handle<Resource> hnd) const
        {
            return resources_.at(hnd.index).get();
        }

    private:
        cache(const cache<Resource>&) = delete;
        cache& operator=(const cache<Resource>&) = delete;

        std::unique_ptr<Resource> load(boost::filesystem::path id)
        {
            std::cout << "loading: " << id << '\n';
            auto resource_path = cache_directory_ / id;
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            auto data = std::make_unique<Resource>();
            ia >> *data;
            return data;
        }

        boost::filesystem::path cache_directory_;
        std::unordered_map<boost::filesystem::path, sigma::handle> handle_map_;

        std::vector<sigma::handle> handles_;
        std::vector<std::uint32_t> free_handles_;
        std::vector<std::unique_ptr<Resource>> resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
