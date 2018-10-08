#ifndef SIGMA_CORE_RESOURCE_CACHE_HPP
#define SIGMA_CORE_RESOURCE_CACHE_HPP

#include <sigma/resource/resource.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <unordered_map>

namespace sigma {
class context;

namespace resource {
    using key_type = boost::filesystem::path;

    class missing_resource : public std::exception {
    public:
        missing_resource(const key_type& key);

        virtual const char* what() const noexcept override;

    private:
        std::string message_;
    };

    class base_cache {
    public:
        base_cache(std::shared_ptr<context> context, const std::string& short_name);

        base_cache(const base_cache&) = delete;

        base_cache(base_cache&&) = default;

        virtual ~base_cache() = default;

        base_cache& operator=(const base_cache&) = delete;

        base_cache& operator=(base_cache&&) = default;

        bool exists(const key_type& key) const;

        std::time_t last_modification_time(const key_type& key) const;

    protected:
        std::shared_ptr<context> context_;
        boost::filesystem::path cache_path_;
    };

    template <class T>
    class cache : public base_cache {
    public:
        cache(std::shared_ptr<context> context)
            : base_cache(context, resource_shortname(T))
            , next_id(1)
        {
        }

        std::shared_ptr<T> insert(const key_type& key, std::shared_ptr<T> r, bool write_to_disk)
        {
            if (write_to_disk) {
                auto path = cache_path_ / key;

                auto p_path = path.parent_path();
                if (!boost::filesystem::exists(p_path))
                    boost::filesystem::create_directories(p_path);

                std::ofstream file { path.string(), std::ios::binary | std::ios::out };
                boost::archive::binary_oarchive oa { file };
                oa << *r;
            }

            return insert_(key, r);
        }

        std::shared_ptr<T> get(const key_type& key)
        {
            auto it = resources_.find(key);
            if (it != resources_.end() && !it->second.second.expired())
                return it->second.second.lock();

            if (!exists(key))
                throw missing_resource(key);

            auto r = std::make_shared<T>(context_, key);
            auto path = cache_path_ / key;
            std::ifstream file { path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive oa { file };
            oa >> *r;

            return insert_(key, r);
        }

    private:
        std::shared_ptr<T> insert_(const key_type& key, std::shared_ptr<T> r)
        {
            auto it = resources_.find(key);
            if (it == resources_.end()) {
                resources_[key] = std::make_pair(next_id, r);
                r->set_id(next_id);
                next_id++;
            } else {
                it->second.second = r;
                r->set_id(it->second.first);
            }
            return r;
        }

        size_t next_id;
        std::unordered_map<key_type, std::pair<size_t, std::weak_ptr<T>>> resources_;
    };
}
}

#endif // SIGMA_CORE_RESOURCE_CACHE_HPP
