#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/resource/identifier.hpp>

#include <boost/archive/binary_iarchive.hpp>

#include <cassert>
#include <fstream>
#include <iostream> // TODO this is bad
#include <unordered_map>

namespace sigma {
namespace resource {
    template <class>
    class manager;

    template <class Resource>
    class handle {
    public:
        handle()
            : manager_(nullptr)
        {
        }

        handle(identifier id)
            : id_(id)
            , manager_(nullptr)
        {
        }

        handle(identifier id, manager<Resource>* m)
            : id_(id)
            , manager_(m)
        {
            assert(is_valid() && "Must fully construct a handle.");
            manager_->reference(id_);
        }

        handle(const handle<Resource>& other)
            : id_(other.id_)
            , manager_(other.manager_)
        {
            if (manager_)
                manager_->reference(id_);
        }

        ~handle()
        {
            if (manager_)
                manager_->dereference(id_);
        }

        handle<Resource>& operator=(const handle<Resource>& other)
        {
            if (id_ != other.id_) {
                if (manager_)
                    manager_->dereference(id_);
                id_ = other.id_;
                if (other.manager_) {
                    manager_ = other.manager_;
                    manager_->reference(id_);
                }
            }
            return *this;
        }

        bool is_valid() const
        {
            return manager_ != nullptr;
        }

        void set_manager(manager<Resource>* m)
        {
            assert(m != nullptr && "Must set a valid manager.");
            if (manager_)
                manager_->dereference(id_);
            manager_ = m;
            manager_->reference(id_);
        }

        Resource* get()
        {
            assert(is_valid() && "Resource handle is not valid.");
            return manager_->acquire(id_);
        }

        const Resource* get() const
        {
            assert(is_valid() && "Resource handle is not valid.");
            return manager_->acquire(id_);
        }

        Resource* operator->()
        {
            assert(is_valid() && "Resource handle is not valid.");
            return manager_->acquire(id_);
        }

        const Resource* operator->() const
        {
            assert(is_valid() && "Resource handle is not valid.");
            return manager_->acquire(id_);
        }

        Resource& operator*()
        {
            assert(is_valid() && "Resource handle is not valid.");
            return *manager_->acquire(id_);
        }

        const Resource& operator*() const
        {
            assert(is_valid() && "Resource handle is not valid.");
            return *manager_->acquire(id_);
        }

        explicit operator bool() const noexcept
        {
            if (is_valid())
                return manager_->is_loaded(id_);
            return false;
        }

        operator identifier() const noexcept
        {
            return id_;
        }

        bool operator==(const handle<Resource>& other) const noexcept
        {
            return id_ == other.id_;
        }

        bool operator!=(const handle<Resource>& other) const noexcept
        {
            return id_ != other.id_;
        }

        bool operator==(std::nullptr_t) const noexcept
        {
            if (manager_)
                return !manager_->is_loaded(id_);
            return true;
        }

        bool operator!=(std::nullptr_t) const noexcept
        {
            if (manager_)
                return manager_->is_loaded(id_);
            return false;
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& id_;
        }

    private:
        identifier id_;
        manager<Resource>* manager_;
    };

    template <class Resource>
    class manager {
    public:
        using resource_type = Resource;
        using resource_data = typename Resource::resource_data;

        manager(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        manager(manager<Resource>&&) = default;

        manager& operator=(manager<Resource>&&) = default;

        virtual ~manager() = default;

        virtual std::unique_ptr<Resource> create(resource_data data) = 0;

        bool is_loaded(identifier id) const
        {
            return resources_data_.count(id) > 0; //reference_counts_.count(id) > 0 && resources_.count(id) > 0 && resources_.at(id) != nullptr;
        }

        bool is_created(identifier id) const
        {
            return resources_.count(id) > 0 && resources_.at(id) != nullptr;
        }

        void reference(identifier id)
        {
            reference_counts_[id]++; // TODO mark for loading
        }

        handle<Resource> get(identifier id)
        {
            return handle<Resource>{ id, this };
        }

        handle<Resource> duplicate(identifier src_id, identifier dest_id)
        {
            if (!is_loaded(src_id))
                load(src_id);
            resources_data_[dest_id] = resources_data_[src_id];
            return { dest_id, this };
        }

        Resource* acquire(identifier id)
        {
            if (is_created(id))
                return resources_[id].get();

            if (!is_loaded(id))
                load(id);

            resources_[id] = std::move(create(resources_data_[id]));

            return resources_[id].get();
        }

        const Resource* acquire(identifier id) const
        {
            if (is_loaded(id))
                return resources_.at(id).get();
            return nullptr;
        }

        void dereference(identifier id)
        {
            if (reference_counts_[id] == 1)
                resources_[id] = nullptr;
            reference_counts_[id]--;
        }

    private:
        manager(const manager<Resource>&) = delete;
        manager& operator=(const manager<Resource>&) = delete;

        void load(identifier id)
        {
            std::cout << "loading: " << id << '\n';
            try {
                auto resource_path = cache_directory_ / std::to_string(id.value());
                std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
                boost::archive::binary_iarchive ia{ file };
                resource_data data;
                ia >> data;
                resources_data_[id] = std::move(data);
            } catch (boost::archive::archive_exception& ex) {
                std::cout << "resource"
                          << ": " << id << " " << ex.what() << '\n';
            } catch (std::exception& ex) {
                std::cout << "resource"
                          << ": " << id << " " << ex.what() << '\n';
            } catch (...) { // TODO check for correct errors here
                std::cout << "resource"
                          << ": " << id << "unknown exception" << '\n';
            }
        }

        boost::filesystem::path cache_directory_;
        std::unordered_map<identifier, std::size_t> reference_counts_;
        std::unordered_map<identifier, resource_data> resources_data_;
        std::unordered_map<identifier, std::unique_ptr<Resource>> resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
