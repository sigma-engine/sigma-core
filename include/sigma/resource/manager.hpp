#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/handle.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/util/json_conversion.hpp>

#include <boost/archive/binary_iarchive.hpp>

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
        using resource_data = typename Resource::resource_data;

        cache(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        cache(cache<Resource>&&) = default;

        cache& operator=(cache<Resource>&&) = default;

        virtual ~cache() = default;

        virtual handle<Resource> get(identifier id)
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

        resource_data* acquire_data(handle<Resource> hnd) const
        {
            return resources_.at(hnd.index).get();
        }

    private:
        cache(const cache<Resource>&) = delete;
        cache& operator=(const cache<Resource>&) = delete;

        std::unique_ptr<resource_data> load(identifier id)
        {
            std::cout << "loading: " << id << '\n';
            auto resource_path = cache_directory_ / std::to_string(id.value());
            std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            auto data = std::make_unique<resource_data>();
            ia >> *data;
            return data;

            // try {
            //
            // } catch (boost::archive::archive_exception& ex) {
            //     std::cout << "resource"
            //               << ": " << id << " " << ex.what() << '\n';
            // } catch (std::exception& ex) {
            //     std::cout << "resource"
            //               << ": " << id << " " << ex.what() << '\n';
            // } catch (...) { // TODO check for correct errors here
            //     std::cout << "resource"
            //               << ": " << id << "unknown exception" << '\n';
            // }
        }

        boost::filesystem::path cache_directory_;
        std::unordered_map<identifier, sigma::handle> handle_map_;

        std::vector<sigma::handle> handles_;
        std::vector<std::uint32_t> free_handles_;
        std::vector<std::unique_ptr<resource_data>> resources_;
    };

    template <class Resource>
    class manager : public cache<Resource> {
    public:
        using resource_type = Resource;
        using resource_data = typename Resource::resource_data;

        manager(boost::filesystem::path cache_directory)
            : cache<Resource>(cache_directory)
        {
        }

        manager(manager<Resource>&&) = default;

        manager& operator=(manager<Resource>&&) = default;

        virtual ~manager() = default;

        virtual std::unique_ptr<Resource> create(resource_data data) = 0;

        handle<Resource> get(identifier id) override
        {
            auto h = cache<Resource>::get(id);

            if (h.index >= rresources_.size())
                rresources_.resize(h.index + 1);

            if (rresources_[h.index] == nullptr)
                rresources_[h.index] = std::move(create(*cache<Resource>::acquire_data(h)));

            return h;
        }

        Resource* acquire(handle<Resource> hnd) const
        {
            return rresources_.at(hnd.index).get();
        }

    private:
        manager(const manager<Resource>&) = delete;
        manager& operator=(const manager<Resource>&) = delete;

        std::vector<std::unique_ptr<Resource>> rresources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
