#ifndef SIGMA_ENGINE_COMPONENT_HPP
#define SIGMA_ENGINE_COMPONENT_HPP

#include <sigma/config.hpp>

#include <unordered_map>
#include <vector>

#include <sigma/entity.hpp>
#include <sigma/util/compile_time_hash.hpp>

#define sigma_COMPONENT(T)                                                    \
    namespace sigma {                                                         \
    template <>                                                                   \
    struct component_type_info<T> {                                               \
        static constexpr const std::size_t type_id = util::compile_time_hash(#T); \
    };                                                                            \
    }

namespace sigma {
template <typename T>
struct component_type_info {
    static_assert(sizeof(component_type_info<T>) != 0, "Missing component_type_info!");
};

template <typename T>
class component_manager {
public:
    virtual bool has(entity e) const = 0;

    virtual T& add(entity e) = 0;

    virtual T& get(entity e) = 0;

    virtual const T& get(entity e) const = 0;
};

template <typename T>
class basic_component_manager : public component_manager<T> {
public:
    virtual bool has(entity e) const override
    {
        // TODO check that entity is alive
        return entities_.count(e) > 0;
    }

    virtual T& add(entity e) override
    {
        // TODO check that entity is alive
        auto it = entities_.find(e);
        if (it != entities_.end())
            return components_.at(it->second);
        entities_[e] = components_.size();
        components_.resize(components_.size() + 1);
        return components_.back();
    }

    virtual T& get(entity e) override { return components_.at(entities_.at(e)); }

    virtual const T& get(entity e) const override
    {
        return components_.at(entities_.at(e));
    }

protected:
    std::unordered_map<entity, std::size_t> entities_;
    std::vector<T> components_;
};
}

#endif // SIGMA_ENGINE_COMPONENT_HPP
