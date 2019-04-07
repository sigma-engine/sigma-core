#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <json/json.h>

#include <filesystem>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace sigma {

namespace resource {
    class base_cache;
    template <class>
    class cache;
}

class base_settings_group
{};

template<class T>
class settings_group : public base_settings_group, public T
{

};

class context : public std::enable_shared_from_this<context> {
public:
    context(const std::filesystem::path& cache_path);

    context(context&&) = default;

    context& operator=(context&&) = default;

    const std::filesystem::path& cache_path() const;

    template <class U>
    inline std::shared_ptr<resource::cache<U>> cache()
    {
        auto it = caches_.find(typeid(U));
        if (it != caches_.end())
            return std::static_pointer_cast<resource::cache<U>>(it->second);
        auto cache = std::make_shared<resource::cache<U>>(cache_path_);
        caches_[typeid(U)] = std::static_pointer_cast<resource::base_cache>(cache);
        return cache;
    }

    template <class U>
    inline std::shared_ptr<settings_group<U>> settings()
    {
        auto it = settings_.find(typeid(U));
        if (it != settings_.end())
            return std::static_pointer_cast<settings_group<U>>(it->second);
        
        auto group = std::make_shared<settings_group<U>>();
        settings_[typeid(U)] = std::static_pointer_cast<base_settings_group>(group);

        auto settings_path = cache_path_ / "settings.json";
        std::ifstream file{ settings_path };
        Json::Value json_settings;
        file >> json_settings;

        if (json_settings.isMember(U::GROUP)) {
            try {
                group->load_settings(shared_from_this(), json_settings[U::GROUP]);
            } catch (const std::exception& e) {
                std::cerr << "error: " << e.what() << '\n';
            }
        }
        return group;
    }

    void write_database();

private:
    context(const context&) = delete;

    context& operator=(const context&) = delete;

    std::filesystem::path cache_path_;
    std::unordered_map<std::type_index, std::shared_ptr<resource::base_cache>> caches_;
    std::unordered_map<std::type_index, std::shared_ptr<base_settings_group>> settings_;
};
}

#endif // SIGMA_CONTEXT_HPP
