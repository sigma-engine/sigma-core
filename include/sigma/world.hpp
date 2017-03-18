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
    static constexpr size_t index_of()
    {
        return index<Component, Components...>::value;
    }

    template <class... SubComponents>
    static component_mask_type mask_for()
    {
        component_mask_type m;
        auto l = { m.set(index_of<SubComponents>())... };
        (void)l;
        return m;
    }
};

template <class T>
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

    T* get(size_t index)
    {
        auto chunk_index = index / count_per_chunk;
        auto chunk_offset = index % count_per_chunk;

        assert(chunk_index < chunks.size());

        return ((T*)chunks[chunk_index]) + chunk_offset;
    }

    template <class... Arguments>
    T* add(size_t index, Arguments&&... args)
    {
        auto chunk_index = index / count_per_chunk;
        if (chunk_index >= chunks.size())
            create_chunk(chunk_index);

        return new (get(index)) T{ std::forward<Arguments>(args)... };
    }

    void remove(size_t index)
    {
        // TODO free chunk when last component is removed from it.
        get(index)->~T();
    }

    void create_chunk(size_t index)
    {
        if (index >= chunks.size())
            chunks.resize(index + 1, nullptr);

        if (chunks[index] == nullptr)
            chunks[index] = new char[sizeof(T) * count_per_chunk];
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

    bool is_alive(entity e) const noexcept
    {
        return e.index >= 0 && e.index < entities.size() && entities[e.index].index != std::uint32_t(-1) && entities[e.index].version == e.version;
    }

    template <class Component, class... Arguments>
    Component* add(entity e, Arguments&&... args)
    {
        assert(is_alive(e));
        assert(!has<Component>(e));

        entity_masks[e.index].set(component_set_type::template index_of<Component>());

        auto& data = std::get<component_set_type::template index_of<Component>()>(component_data);
        return data.add(e.index, std::forward<Arguments>(args)...);
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
        auto& data = std::get<component_set_type::template index_of<Component>()>(component_data);
        return data.get(e.index);
    }

    template <class Component>
    void remove(entity e)
    {
        assert(is_alive(e));
        if (has<Component>(e)) {
			entity_masks[e.index].set(component_set_type::template index_of<Component>(), false);
            auto& data = std::get<component_set_type::template index_of<Component>()>(component_data);
            data.remove(e.index);
        }
    }

    void remove_components(entity e)
    {
        using expand_type = int[];
        expand_type{ (world::template remove<Components>(e), 0)... };
    }

    void destroy(entity e)
    {
        if (!is_alive(e))
            return;
        remove_components(e);
        free_entities.push_back(entities[e.index].index);
        entities[e.index].index = std::uint32_t(-1);
    }

    template <class... SubComponents, class F>
    void for_each(F f)
    {
        auto mask = component_set_type::template mask_for<SubComponents...>();
        auto count = entities.size();
        for (std::size_t i = 0; i < count; ++i) {
            auto e = entities[i];
            if (is_alive(e) && ((entity_masks[e.index] & mask) == mask))
                f(e, *(SubComponents*)(std::get<component_set_type::template index_of<SubComponents>()>(component_data).get(e.index))...);
        }
    }

private:
    world(const world<Components...>&) = delete;
    world<Components...>& operator=(const world<Components...>&) = delete;

    std::vector<entity> entities;
    std::vector<std::uint32_t> free_entities;
    std::vector<component_mask_type> entity_masks;
    std::tuple<component_storage<Components>...> component_data;
};
}

#endif // SIGMA_WORLD_HPP
