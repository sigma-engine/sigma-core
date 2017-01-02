#ifndef SIGMA_ENGINE_RESOURCE_CACHE_HPP
#define SIGMA_ENGINE_RESOURCE_CACHE_HPP

#include <sigma/resource/identifier.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <iostream> // TODO remove this
#include <unordered_map>

namespace sigma {
namespace resource {

    template <class T>
    class cache {
    public:
        class instance {
        public:
            instance(){}

            instance(identifier id)
                : id_(id)
                , data_(nullptr)
            {
            }

            instance(identifier id, std::shared_ptr<T> data)
                : id_(id)
                , data_(data)
            {
            }

            instance& operator =(identifier id) {
                if(id_ != id) {
                    id_ = id;
                    data_ = nullptr;
                }
            }

            ~instance() = default;

            bool operator==(const instance& other) const noexcept
            {
                return id_ == other.id_;
            }

            bool operator!=(const instance& other) const noexcept
            {
                return id_ != other.id_;
            }

            explicit operator bool() const noexcept
            {
                return data_.operator bool();
            }

            operator identifier() const noexcept
            {
                return id_;
            }

            T* operator->()
            {
                return data_.get();
            }

            const T* operator->() const
            {
                return data_.get();
            }

            T& operator*()
            {
                return *data_;
            }

            const T& operator*() const
            {
                return *data_;
            }

            void patch(std::shared_ptr<T> data)
            {
                data_ = data;
            }

            identifier id() const {
                return id_;
            }

        private:
            identifier id_;
            std::shared_ptr<T> data_;

            friend class boost::serialization::access;
            template <class Archive>
            void serialize(Archive& ar, const unsigned int version)
            {
                ar& id_;
            }
        };

        cache(boost::filesystem::path cache_directory)
            : cache_directory_(cache_directory)
        {
        }

        virtual ~cache() = default;

        virtual void patch(std::shared_ptr<T> resource)
        {
        }

        instance get(identifier resource_id)
        {
            if (resources_.count(resource_id) > 0 && !resources_[resource_id].expired())
                return { resource_id, resources_[resource_id].lock() };

            auto new_resource = std::make_shared<T>();
            try {
                auto resource_path = cache_directory_ / std::to_string(resource_id.value());
                std::ifstream file{ resource_path.string(), std::ios::binary | std::ios::in };
                boost::archive::binary_iarchive ia{ file };
                ia >> *new_resource;
                patch(new_resource);
                resources_.emplace(resource_id, new_resource);
                return { resource_id, new_resource };
            } catch (boost::archive::archive_exception& ex) {
                std::cout << "resource"
                          << ": " << resource_id << " " << ex.what() << std::endl;
            } catch (std::exception& ex) {
                std::cout << "resource"
                          << ": " << resource_id << " " << ex.what() << std::endl;
            } catch (...) { // TODO check for correct errors here
                std::cout << "resource"
                          << ": " << resource_id << "unknown exception" << std::endl;
            }
            return {};
        }

    protected:
        boost::filesystem::path cache_directory_;
        std::unordered_map<identifier, std::weak_ptr<T> > resources_;

    private:
        cache(const cache<T>&) = delete;
        cache& operator=(const cache<T>&) = delete;
    };
}
}

#endif //SIGMA_ENGINE_RESOURCE_CACHE_HPP
