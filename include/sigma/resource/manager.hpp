#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/resource/identifier.hpp>

#include <boost/archive/binary_iarchive.hpp>

#include <fstream>
#include <iostream> // TODO this is bad
#include <cassert>
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
				id_ = other.id_;
                if (manager_)
                    manager_->dereference(id_);
				if (other.manager_) {
					manager_ = other.manager_;
					manager_->reference(id_);
				}
            }
			return *this;
        }

		bool is_valid() const {
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

    private:
        identifier id_;
        manager<Resource>* manager_;
    };

    template <class Resource>
    class manager {
    public:
        using resource_data = typename Resource::resource_data;

        manager(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        manager(manager<Resource>&&) = default;

        manager& operator=(manager<Resource>&&) = default;

        virtual ~manager() = default;

        virtual std::unique_ptr<Resource> load(resource_data data, boost::archive::binary_iarchive& ia) = 0;

        bool is_loaded(identifier id) const
        {
            return reference_counts_.count(id) > 0 && resources_.count(id) > 0 && resources_.at(id) != nullptr;
        }

        void reference(identifier id)
        {
            reference_counts_[id]++; // TODO mark for loading
        }

        handle<Resource> get(identifier id)
        {
            return handle<Resource>{ id, this };
        }

        Resource* acquire(identifier id)
        {
            if (is_loaded(id))
                return resources_[id].get();
            if (reference_counts_[id] > 0) {
                try {
                    auto resource_path = cache_directory_ / std::to_string(id.value());
                    std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
                    boost::archive::binary_iarchive ia{ file };
                    resource_data data;
                    ia >> data;
                    resources_[id] = std::move(load(std::move(data), ia));
                    return resources_[id].get();
                } catch (boost::archive::archive_exception& ex) {
                    std::cout << "resource"
                              << ": " << id << " " << ex.what() << std::endl;
                } catch (std::exception& ex) {
                    std::cout << "resource"
                              << ": " << id << " " << ex.what() << std::endl;
                } catch (...) { // TODO check for correct errors here
                    std::cout << "resource"
                              << ": " << id << "unknown exception" << std::endl;
                }
            }
            return nullptr;
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

        boost::filesystem::path cache_directory_;
        std::unordered_map<identifier, std::size_t> reference_counts_;
        std::unordered_map<identifier, std::unique_ptr<Resource> > resources_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP
