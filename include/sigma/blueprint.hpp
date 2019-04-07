#ifndef SIGMA_BLUEPRINT_HPP
#define SIGMA_BLUEPRINT_HPP

#include <sigma/component.hpp>
#include <sigma/resource/resource.hpp>

#include <entt/entt.hpp>

namespace sigma {
template <class ComponentSet>
class blueprint;

template <class... Components>
class blueprint<type_set<Components...>> {
public:
    entt::registry<> registry;

    void save(cereal::BinaryOutputArchive & ar) const
    {
        registry.snapshot()
        .entities(ar)
        .destroyed(ar)
        .component<Components...>(ar);
    }

    void load(cereal::BinaryInputArchive &ar)
    {
        registry.loader()
        .entities(ar)
        .destroyed(ar)
        .component<Components...>(ar)
        .orphans();
    }
};

namespace resource {
    template <class... Components>
    struct resource_traits<blueprint<Components...>> {
        static constexpr const char* fullname = "sigma::blueprint";
        static constexpr const char* shortname = "blueprint";
    };
}
}

#endif // SIGMA_BLUEPRINT_HPP
