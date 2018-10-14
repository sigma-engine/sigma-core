#ifndef SIGMA_CORE_RESOURCE_CACHE_HPP
#define SIGMA_CORE_RESOURCE_CACHE_HPP

#include <sigma/resource/resource.hpp>

#define CEREAL_FUTURE_EXPERIMENTAL
#include <cereal/archives/adapters.hpp>
#include <cereal/archives/binary.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <unordered_map>

namespace sigma {
class context;

namespace resource {
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
        std::weak_ptr<context> context_;
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

        void write_to_disk(const key_type& key)
        {
            auto path = cache_path_ / key;

            auto p_path = path.parent_path();
            if (!boost::filesystem::exists(p_path))
                boost::filesystem::create_directories(p_path);

            std::ofstream file { path.string(), std::ios::binary | std::ios::out };
            auto ctx = context_.lock();
            cereal::UserDataAdapter<std::shared_ptr<context>, cereal::BinaryOutputArchive> oa(ctx, file);
            oa(*get(key));
        }

        handle_type<T> insert(const key_type& key, std::shared_ptr<T> r, bool should_write = false)
        {
            handle_type<T> h = insert_(key, r);
            if (should_write) {
                write_to_disk(key);
            }

            return h;
        }

        handle_type<T> get(const key_type& key)
        {
            auto it = resources_.find(key);
            if (it != resources_.end() && !it->second.second.expired())
                return handle_type<T> { it->second.second.lock() };

            if (!exists(key))
                throw missing_resource(key);

            auto r = std::make_shared<T>(context_, key);
            auto path = cache_path_ / key;
            auto ctx = context_.lock();
            std::ifstream file { path.string(), std::ios::binary | std::ios::in };
            cereal::UserDataAdapter<std::shared_ptr<context>, cereal::BinaryInputArchive> ia(ctx, file);
            ia(*r);

            return insert_(key, r);
        }

    private:
        handle_type<T> insert_(const key_type& key, std::shared_ptr<T> r)
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
            return handle_type<T> { r };
        }

        size_t next_id;
        std::unordered_map<key_type, std::pair<size_t, std::weak_ptr<T>>> resources_;
    };
}
}

#endif // SIGMA_CORE_RESOURCE_CACHE_HPP
