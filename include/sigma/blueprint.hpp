#ifndef SIGMA_BLUEPRINT_HPP
#define SIGMA_BLUEPRINT_HPP

#include <sigma/component.hpp>
#include <sigma/resource/resource.hpp>

#include <locale>

#include <boost/variant.hpp>
#include <cereal/types/boost_variant.hpp>

namespace sigma {
template <class ComponentSet>
class blueprint;

template <class... Components>
class blueprint<type_set<Components...>> {
public:
    using entity_type = std::vector<boost::variant<Components...>>;
    using component_set_type = component_set<Components...>;

    std::vector<entity_type> entities;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(entities);
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
