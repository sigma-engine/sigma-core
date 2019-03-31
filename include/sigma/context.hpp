#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <filesystem>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace sigma {

namespace resource {
    class base_cache;
    template <class>
    class cache;
}

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
        auto cache = std::make_shared<resource::cache<U>>(shared_from_this());
        caches_[typeid(U)] = std::static_pointer_cast<resource::base_cache>(cache);
        return cache;
    }

private:
    context(const context&) = delete;

    context& operator=(const context&) = delete;

    std::filesystem::path cache_path_;
    std::unordered_map<std::type_index, std::shared_ptr<resource::base_cache>> caches_;
};
}

#endif // SIGMA_CONTEXT_HPP
