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

    template <class Resource>
    struct resource_loader {
        typedef boost::filesystem::path identifier_type;
        std::unique_ptr<Resource> operator()(const boost::filesystem::path& cache_directory, const identifier_type& id) const
        {
            auto resource_path = cache_directory / id;
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            auto data = std::make_unique<Resource>();
            ia >> *data;
            return data;
        }
    };

    template <class Resource>
    class cache {
    public:
        typedef resource_loader<Resource> loader_type;
        typedef typename loader_type::identifier_type identifier_type;

        cache(const boost::filesystem::path& cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        virtual ~cache() = default;

        virtual handle<Resource> get(const identifier_type& id)
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
        cache(const cache<Resource>&) = delete;
        cache& operator=(const cache<Resource>&) = delete;

        loader_type loader_;

        boost::filesystem::path cache_directory_;
        std::unordered_map<identifier_type, sigma::handle> handle_map_;

        std::vector<sigma::handle> handles_;
        std::vector<std::uint32_t> free_handles_;
        // TODO remove the use of unique_ptr
        std::vector<std::unique_ptr<Resource>> resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
