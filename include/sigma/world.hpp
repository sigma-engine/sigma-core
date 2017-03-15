#ifndef SIGMA_WORLD_HPP
#define SIGMA_WORLD_HPP

#include <sigma/entity.hpp>
#include <sigma/util/variadic.hpp>

#include <array>
#include <bitset>
#include <cassert>
#include <vector>

namespace sigma {
struct component_storage {
    std::vector<char*> chunks;
    size_t count_per_chunk = 256;

    ~component_storage()
    {
        for (char* chunk : chunks) {
            if (chunk)
                delete[] chunk;
        }
    }

    template <class T>
    T* get(size_t index)
    {
        auto chunk_index = index / count_per_chunk;
        auto chunk_offset = index % count_per_chunk;

        assert(chunk_index < chunks.size());

        return ((T*)chunks[chunk_index]) + chunk_offset;
    }

    template <class T, class... Arguments>
    T* add(size_t index, Arguments&&... args)
    {
        auto chunk_index = index / count_per_chunk;
        if (chunk_index >= chunks.size())
            create_chunk<T>(chunk_index);

        return new (get<T>(index)) T{ std::forward<Arguments>(args)... };
    }

    template <class T>
    void remove(size_t index)
    {
        // TODO free chunk when last component is removed from it.
        get<T>(index)->~T();
    }

    template <class T>
    void create_chunk(size_t index)
    {
        if (index >= chunks.size())
            chunks.resize(index + 1, nullptr);

        if (chunks[index] == nullptr)
            chunks[index] = new char[sizeof(T) * count_per_chunk];
    }
};

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

    template <class Component, class... Arguments>
    Component* add(entity e, Arguments&&... args)
    {
        if (e.index >= entity_masks.size())
            entity_masks.resize(e.index + 1);

        entity_masks[e.index] |= component_mask_type(1 << index_of<Component>());

        component_storage& data = component_data[index_of<Component>()];
        return data.template add<Component>(e.index, std::forward<Arguments>(args)...);
    }

    template <class Component>
    bool has(entity e) const
    {
        auto comp_mask = mask_for<Component>();
        return e.index < entity_masks.size() && ((entity_masks[e.index] & comp_mask) == comp_mask);
    }

    template <class... SubComponents>
    void update_mask(entity e, std::bitset<sizeof...(SubComponents)>& mask) const
    {
        if (e.index < entity_masks.size()) {
            auto l = { (mask[component_set<SubComponents...>::template index_of<SubComponents>()] = entity_masks[e.index][index_of<SubComponents>()])... };
            (void)l;
        }
    }

    template <class Component>
    Component* get(entity e)
    {
        assert(e.index < entity_masks.size());
        assert(has<Component>(e));

        component_storage& data = component_data[index_of<Component>()];
        return data.template get<Component>(e.index);
    }

    template <class Component>
    void remove(entity e)
    {
        if (has<Component>(e)) {
            entity_masks[e.index] &= ~component_mask_type(1 << index_of<Component>());
            component_storage& data = component_data[index_of<Component>()];
            data.template remove<Component>(e.index);
        }
    }

    void remove_components(entity e)
    {
        using expand_type = int[];
        expand_type{ (remove<Components>(e), 0)... };
    }

    std::vector<component_mask_type> entity_masks;
    std::array<component_storage, sizeof...(Components)> component_data;
};

template <class... Components>
struct world : public component_set<Components...> {

    world() = default;
    world(world<Components...>&&) = default;
    world<Components...>& operator=(world<Components...>&&) = default;

    entity create()
    {
        if (free_entities.empty()) {
            entity e{ std::uint32_t(entities.size()), 0 };
            entities.push_back(e);
            return e;
        } else {
            auto index = free_entities.back();
            free_entities.pop_back();
            entities[index].index = index;
            entities[index].version++;
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

        return component_set<Components...>::template add<Component>(e, std::forward<Arguments>(args)...);
    }

    template <class Component>
    bool has(entity e)
    {
        if (!is_alive(e))
            return false;
        return component_set<Components...>::template has<Component>(e);
    }

    template <class Component>
    Component* get(entity e)
    {
        assert(is_alive(e));
        assert(has<Component>(e));
        return component_set<Components...>::template get<Component>(e);
    }

    template <class Component>
    void remove(entity e)
    {
        assert(is_alive(e));
        if (has<Component>(e)) {
            component_set<Components...>::template remove<Component>(e);
        }
    }

    void remove_components(entity e)
    {
        component_set<Components...>::remove_components(e);
    }

    void destroy(entity e)
    {
        if (!is_alive(e) || e.index >= entities.size())
            return;
        remove_components(e);
        free_entities.push_back(entities[e.index].index);
        entities[e.index].index = std::uint32_t(-1);
    }

    template <class... SubComponents>
    bool has_all(entity e) const
    {
        std::bitset<sizeof...(SubComponents)> mask;
        component_set<Components...>::template update_mask<SubComponents...>(e, mask);
        return mask.all();
    }

    template <class... SubComponents, class F>
    void for_each(F f)
    {
        // TODO this still loops over entities that are dead.
        auto count = entities.size();
        for (std::size_t i = 0; i < count; ++i) {
            auto e = entities[i];
            if (is_alive(e) && has_all<SubComponents...>(e))
                f(e, *get<SubComponents>(e)...);
        }
    }

private:
    world(const world<Components...>&) = delete;
    world<Components...>& operator=(const world<Components...>&) = delete;

    std::vector<entity> entities;
    std::vector<std::uint32_t> free_entities;
};
}

#endif // SIGMA_WORLD_HPP
