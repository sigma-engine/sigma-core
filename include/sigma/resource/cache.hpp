#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/config.hpp>
#include <sigma/resource/resource.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/functional/hash.hpp>

#include <filesystem>
#include <exception>
#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace resource {
    using resource_id = std::vector<std::filesystem::path>;

    class missing_resource : public std::exception {
    public:
        missing_resource(const resource::resource_id& cid)
        {
            std::stringstream ss;

            ss << "missing resource ";
            if (cid.size() >= 2) {
                ss << "{ " << cid[0];
                for (std::size_t i = 1; i < cid.size() - 1; ++i) {
                    if (!cid[i].empty())
                        ss << cid[i] << ", ";
                }
                ss << cid[cid.size() - 1] << "}";
            } else if (cid.size() == 1) {
                ss << cid[0];
            } else {
                ss << "<UNKNOWN>";
            }
            message_ = ss.str();
        }

        virtual const char* what() const noexcept override
        {
            return message_.c_str();
        }

    private:
        std::string message_;
    };

    template <class Resource>
    class cache {
    public:
        cache(const std::filesystem::path& cache_directory)
            : cache_directory_(cache_directory)
        {
            auto database_path = cache_directory_ / "database";
            if (!std::filesystem::exists(cache_directory_)) {
                std::filesystem::create_directories(cache_directory_);
            } else if (std::filesystem::exists(database_path)) {
                std::ifstream file{ database_path.string(), std::ios::binary | std::ios::in };
                boost::archive::binary_iarchive ia{ file };
                ia >> database_;
            }
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        ~cache() = default;

        bool contains(const resource_id& rid) const
        {
            return database_.count(rid) > 0;
        }

        handle<Resource> handle_for(const resource_id& rid) const
        {
            if (!contains(rid))
                throw missing_resource(rid);

            return database_.at(rid);
        }

        std::filesystem::file_time_type last_modification_time(const handle<Resource>& hnd) const
        {
            auto resource_path = cache_directory_ / std::to_string(hnd.index);
            if (std::filesystem::exists(resource_path))
                return std::filesystem::last_write_time(resource_path);
            return {};
        }

        handle<Resource> insert(const resource_id& rid, Resource resource, bool write_to_disk)
        {
            handle<Resource> hnd;
            if (contains(rid)) {
                hnd = database_.at(rid);
            } else {
                hnd = next_free_handle();
                database_[rid] = hnd;
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

        Resource load(const std::filesystem::path& cache_directory, const handle<Resource>& hnd) const
        {
            assert(hnd.is_valid());
            auto resource_path = cache_directory / std::to_string(hnd.index);
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            Resource data;
            ia >> data;
            return std::move(data);
        }

        std::filesystem::path cache_directory_;
        std::vector<std::pair<handle<Resource>, std::unique_ptr<Resource>>> resources_;
        std::unordered_map<resource_id, handle<Resource>> database_;
    };
}
}

namespace std {
template <>
struct hash<sigma::resource::resource_id> {
    size_t operator()(const sigma::resource::resource_id& rid) const
    {
        size_t hash_code = 0;
        for (const auto& id : rid) {
            if (!id.empty())
                boost::hash_combine(hash_code, id);
        }
        return hash_code;
    }
};
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
