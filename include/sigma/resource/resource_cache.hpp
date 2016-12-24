#ifndef SIGMA_RESOURCE_RESOURCE_CACHE_HPP
#define SIGMA_RESOURCE_RESOURCE_CACHE_HPP

#include <sigma/resource/identifier.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <iostream> // TODO remove this
#include <unordered_map>

namespace sigma {
namespace resource {
    template <typename T>
    class resource_cache {
    public:
        resource_cache(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        virtual ~resource_cache() = default;

        resource_cache(const resource_cache&) = delete;

        resource_cache(resource_cache&&) = default;

        resource_cache& operator=(const resource_cache&) = delete;

        resource_cache& operator=(resource_cache&&) noexcept = default;

        /**
		* @brief Returns if resource is loaded in this cache.
		*
		* @param resource_id the resource to check if cached.
		* @return true if the resource is cache.
		*/
        virtual bool is_cached(resource::identifier resource_id) const
        {
            return resources_.count(resource_id) > 0;
        }

        virtual bool is_loaded(resource::identifier resource_id) const
        {
            return (resources_.count(resource_id) > 0) && resources_.at(resource_id).reference_count > 0;
        }

        /**
		* @brief Increases the reference count associated with the resource.
		*
		*
		* @param resource_id the resource to increase the reference count of.
		* @return true if the resource exists and is valid.
		*/
        virtual bool increment_reference(resource::identifier resource_id)
        {
            // TODO replace std::cout with something better.
            if (is_cached(resource_id)) {
                resources_[resource_id].reference_count++;
                return true;
            }

            T new_resource;
            try {
                auto resource_path = cache_directory_ / std::to_string(resource_id.value());
                std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
                boost::archive::binary_iarchive ia{ file };
                ia >> new_resource;
            } catch (boost::archive::archive_exception& ex) {
                std::cout << "resource"
                          << ": " << resource_id << " " << ex.what() << std::endl;
                return false;
            } catch (std::exception& ex) {
                std::cout << "resource"
                          << ": " << resource_id << " " << ex.what() << std::endl;
                return false;
            } catch (...) { // TODO check for correct errors here
                std::cout << "resource"
                          << ": " << resource_id << "unknown exception" << std::endl;
                return false;
            }

            new_resource.reference_count = 1;
            resources_.emplace(resource_id, std::move(new_resource));

            return true;
        }

        T& get(resource::identifier resource_id)
        {
            return resources_[resource_id];
        }

        /**
		* @brief Decreases the reference count associated with
		* the resource.
		*
		*
		* @param resource_id the resource to decrease the reference count of.
		* @returns true if the resource reference count is zero.
		*/
        virtual bool decrement_reference(resource::identifier resource_id)
        {
            if (!is_cached(resource_id))
                return false;
            auto& resource = resources_[resource_id];
            if (resource.reference_count > 0)
                resource.reference_count--;
            return resource.reference_count == 0;
        }

        virtual void free_unreferenced()
        {
            // TODO algorithm
            for (auto it = std::begin(resources_); it != std::end(resources_);) {
                if (it->second.reference_count == 0)
                    it = resources_.erase(it);
                else
                    ++it;
            }
        }

    protected:
        boost::filesystem::path cache_directory_;
        std::unordered_map<resource::identifier, T> resources_;
    };
}
}

#endif // SIGMA_RESOURCE_RESOURCE_CACHE_HPP
