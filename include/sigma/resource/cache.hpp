#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/config.hpp>
#include <sigma/handle.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <memory>

namespace sigma {
namespace resource {

    template <class>
    using handle = sigma::handle;

    template <class Resource>
    class cache {
    public:
        cache(const boost::filesystem::path& cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        virtual ~cache() = default;

        Resource* acquire(const handle<Resource>& hnd)
        {
            assert(hnd.is_valid());
            if (hnd.index >= resources_.size())
                resources_.resize(hnd.index + 1);

            resources_[hnd.index] = std::move(load(cache_directory_, hnd));

            return resources_[hnd.index].get();
        }

    private:
        cache(const cache<Resource>&) = delete;
        cache& operator=(const cache<Resource>&) = delete;

        std::unique_ptr<Resource> load(const boost::filesystem::path& cache_directory, const handle<Resource>& hnd) const
        {
            auto resource_path = cache_directory / std::to_string(hnd.index);
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            auto data = std::make_unique<Resource>();
            ia >> *data;
            return data;
        }

        boost::filesystem::path cache_directory_;
        std::vector<std::unique_ptr<Resource>> resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
