#ifndef SIGMA_ENGINE_RESOURCE_BASIC_CACHE_HPP
#define SIGMA_ENGINE_RESOURCE_BASIC_CACHE_HPP

#include <sigma/resource/identifier.hpp>

#include <fstream>
#include <unordered_map>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace sigma {
namespace resource {
    template <typename T>
    class basic_cache {
    public:
        basic_cache(boost::filesystem::path data_dir)
            : data_dir_(data_dir)
        {
        }

        bool is_loaded(resource::identifier resource) const
        {
            return resource_map_.count(resource) > 0;
        }

        bool exists(resource::identifier resource) const
        {
            if (is_loaded(resource))
                return true;
            return boost::filesystem::exists(
                data_dir_ / std::to_string(static_cast<std::size_t>(resource)));
        }

        bool load(resource::identifier resource)
        {
            if (is_loaded(resource))
                return true;

            auto path = data_dir_ / std::to_string(static_cast<std::size_t>(resource));
            if (!boost::filesystem::exists(path))
                return false;

            // TODO look into what errors binary_iarchive can throw and catch them
            std::ifstream source_file{ path.string() };
            boost::archive::binary_iarchive ia{ source_file };
            T res;
            ia >> res;
            resource_map_[resource] = resources_.size();
            resources_.push_back(std::move(res));
            return true;
        }

        const T& get(resource::identifier resource) const
        {
            return resources_.at(resource_map_.at(resource));
        }

        T& get(resource::identifier resource)
        {
            return resources_.at(resource_map_.at(resource));
        }

    protected:
        std::unordered_map<resource::identifier, std::size_t> resource_map_;
        std::vector<T> resources_;

    private:
        boost::filesystem::path data_dir_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_BASIC_RESOURCE_CACHE_HPP
