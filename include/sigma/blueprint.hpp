#ifndef SIGMA_BLUEPRINT_HPP
#define SIGMA_BLUEPRINT_HPP

#include <sigma/component.hpp>

#include <variant>

namespace sigma {
template <class ComponentSet>
class blueprint;

template <class... Components>
class blueprint<type_set<Components...>> {
public:
    using entity_type = std::vector<std::variant<Components...>>;
    using component_set_type = component_set<Components...>;

    std::vector<entity_type> entities;
};
}

#endif // SIGMA_BLUEPRINT_HPP
