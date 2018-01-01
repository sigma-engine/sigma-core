#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/config.hpp>
#include <sigma/resource/resource.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>

#include <fstream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace resource {

    template <class Resource>
    class cache {
    public:
        cache(const boost::filesystem::path& cache_directory)
            : cache_directory_(cache_directory)
        {
            auto database_path = cache_directory_ / "database";
            if (!boost::filesystem::exists(cache_directory_)) {
                boost::filesystem::create_directories(cache_directory_);
            } else if (boost::filesystem::exists(database_path)) {
                std::ifstream file{ database_path.string(), std::ios::binary | std::ios::in };
                boost::archive::binary_iarchive ia{ file };
                ia >> database_;
            }
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        ~cache() = default;

        bool contains(std::size_t resource_id) const
        {
            return database_.count(resource_id) > 0;
        }

        handle<Resource> handle_for(std::size_t resource_id) const
        {
            return database_.at(resource_id);
        }

        std::time_t last_modification_time(const handle<Resource>& hnd) const
        {
            auto resource_path = cache_directory_ / std::to_string(hnd.index);
            if (boost::filesystem::exists(resource_path))
                return boost::filesystem::last_write_time(resource_path);
            return 0;
        }

        handle<Resource> insert(std::size_t resource_id, Resource resource, bool write_to_disk)
        {
            handle<Resource> hnd;
            if (contains(resource_id)) {
                hnd = database_.at(resource_id);
            } else {
                hnd = next_free_handle();
                database_[resource_id] = hnd;
            }

            assert(hnd.is_valid());

            if (write_to_disk) {
                auto resource_path = cache_directory_ / std::to_string(hnd.index);
                std::ofstream file{ resource_path.string(), std::ios::binary | std::ios::out };
                boost::archive::binary_oarchive oa{ file };
                oa << resource;
            }

            insert_(hnd, std::move(resource));

            return hnd;
        }

        Resource* acquire(const handle<Resource>& hnd)
        {
            assert(hnd.is_valid());
            if (hnd.index >= resources_.size() || resources_[hnd.index].second == nullptr)
                insert_(hnd, std::move(load(cache_directory_, hnd)));
            return resources_[hnd.index].second.get();
        }

        void save() const
        {
            auto database_path = cache_directory_ / "database";
            std::ofstream file{ database_path.string(), std::ios::binary | std::ios::out };
            boost::archive::binary_oarchive oa{ file };
            oa << database_;
        }

    private:
        cache(const cache<Resource>&) = delete;
        cache& operator=(const cache<Resource>&) = delete;

        handle<Resource> next_free_handle() const
        {
            for (std::size_t i = 0; i < resources_.size(); ++i) {
                if (resources_[i].second == nullptr)
                    return resources_[i].first.bump_version();
            }
            assert(resources_.size() < std::numeric_limits<std::size_t>::max());
            return { std::uint32_t(resources_.size()), 0 };
        }

        void insert_(const handle<Resource>& hnd, Resource res)
        {
            assert(hnd.is_valid());
            if (hnd.index >= resources_.size())
                resources_.resize(hnd.index + 1);
            resources_[hnd.index].first = hnd;
            resources_[hnd.index].second = std::make_unique<Resource>(std::move(res));
        }

        Resource load(const boost::filesystem::path& cache_directory, const handle<Resource>& hnd) const
        {
            assert(hnd.is_valid());
            auto resource_path = cache_directory / std::to_string(hnd.index);
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            Resource data;
            ia >> data;
            return std::move(data);
        }

        boost::filesystem::path cache_directory_;
        std::vector<std::pair<handle<Resource>, std::unique_ptr<Resource>>> resources_;
        std::unordered_map<std::size_t, handle<Resource>> database_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
