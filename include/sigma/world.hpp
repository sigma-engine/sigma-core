#ifndef SIGMA_WORLD_HPP
#define SIGMA_WORLD_HPP

#include <sigma/entity.hpp>
#include <sigma/util/variadic.hpp>

#include <array>
#include <bitset>
#include <cassert>
#include <vector>

namespace sigma {
template <class... Components>
struct component_set {
    using component_mask_type = std::bitset<sizeof...(Components)>;

    template <class Component>
    static size_t index_of()
    {
        return index<Component, Components...>::value;
    }

    template <class... SubComponents>
    static component_mask_type mask_for()
    {
        component_mask_type m;
        auto l = { m |= (1 << index_of<SubComponents>())... };
        (void)l;
        return m;
    }
};

struct component_storage {
    size_t size = 0;
    std::vector<char> data;

    template <class T>
    T* get(size_t index)
    {
        return (T*)(data.data() + (index * sizeof(T)));
    }

    template <class T, class... Arguments>
    T* add(size_t index, Arguments&&... args)
    {
        if (index >= size)
            resize<T>(index + 1);
        return new (get<T>(index)) T{ std::forward<Arguments>(args)... };
    }

    template <class T>
    void remove(size_t index)
    {
        get<T>(index)->~T();
    }

    template <class T>
    void resize(size_t size)
    {
        this->size = size;
        this->data.resize(this->size * sizeof(T));
    }
};

template <class... Components>
struct world {
    using component_set_type = component_set<Components...>;
    using component_mask_type = typename component_set<Components...>::component_mask_type;

    world() = default;
    world(world<Components...>&&) = default;
    world<Components...>& operator=(world<Components...>&&) = default;

    entity create()
    {
        if (free_entities.empty()) {
            entity e{ std::uint32_t(entities.size()), 0 };
            entities.push_back(e);
            entity_masks.push_back(0);
            return e;
        } else {
            auto index = free_entities.back();
            free_entities.pop_back();
            entities[index].index = index;
            entities[index].version++;
            entity_masks[index] = 0;
            return entities[index];
        }
    }

    ~world()
    {
        for (auto e : entities) {
            if (is_alive(e))
                remove_components(e);
        }
    }

    bool is_alive(entity e) const
    {
        if (!e.is_valid() || e.index >= entities.size())
            return false;
        return entities[e.index] == e;
    }

    template <class Component, class... Arguments>
    Component* add(entity e, Arguments&&... args)
    {
        assert(is_alive(e));
        assert(!has<Component>(e));

        entity_masks[e.index] |= component_mask_type(1 << component_set_type::template index_of<Component>());

        component_storage& data = component_data[component_set_type::template index_of<Component>()];
        return data.template add<Component>(e.index, std::forward<Arguments>(args)...);
    }

    template <class Component>
    bool has(entity e)
    {
        if (!is_alive(e))
            return false;
        auto comp_mask = component_set_type::template mask_for<Component>();
        return (entity_masks[e.index] & comp_mask) == comp_mask;
    }

    template <class Component>
    Component* get(entity e)
    {
        assert(is_alive(e));
        assert(has<Component>(e));
        component_storage& data = component_data[component_set_type::template index_of<Component>()];
        return data.template get<Component>(e.index);
    }

    template <class Component>
    void remove(entity e)
    {
        assert(is_alive(e));
        if (has<Component>(e)) {
            entity_masks[e.index] &= ~component_mask_type(1 << component_set_type::template index_of<Component>());
            component_storage& data = component_data[component_set_type::template index_of<Component>()];
            data.template remove<Component>(e.index);
        }
    }

    void remove_components(entity e)
    {
        using expand_type = int[];
        expand_type{ (world::template remove<Components>(e), 0)... };
    }

    void destroy(entity e)
    {
        if (!is_alive(e) || e.index >= entities.size())
            return;
        remove_components(e);
        free_entities.push_back(entities[e.index].index);
        entities[e.index].index = std::uint32_t(-1);
    }

    template <class... SubComponents, class F>
    void for_each(F f)
    {
        // TODO this still loops over entities that are dead.
        auto mask = component_set_type::template mask_for<SubComponents...>();
        auto count = entities.size();
        for (std::size_t i = 0; i < count; ++i) {
            auto e = entities[i];
            if (is_alive(e) && (entity_masks[e.index] & mask) == mask)
                f(e, *(SubComponents*)(component_data[component_set_type::template index_of<SubComponents>()].template get<SubComponents>(e.index))...);
        }
    }

private:
    world(const world<Components...>&) = delete;
    world<Components...>& operator=(const world<Components...>&) = delete;

    std::vector<entity> entities;
    std::vector<std::uint32_t> free_entities;
    std::vector<component_mask_type> entity_masks;
    std::array<component_storage, sizeof...(Components)> component_data;
};
}

#endif // SIGMA_WORLD_HPP
