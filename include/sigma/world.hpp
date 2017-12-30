#ifndef SIGMA_WORLD_HPP
#define SIGMA_WORLD_HPP

#include <sigma/config.hpp>
#include <sigma/handle.hpp>
#include <sigma/util/type_sequence.hpp>

#include <boost/serialization/split_member.hpp>

#include <array>
#include <cassert>
#include <tuple>
#include <vector>

namespace sigma {
using entity = handle;

template <class... Components>
using component_set = type_set_t<Components...>;

using component_mask_type = std::uint64_t;

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

    T& get(size_t index)
    {
        auto chunk_index = index / count_per_chunk;
        auto chunk_offset = index % count_per_chunk;

        assert(chunk_index < chunks.size());

        return *(((T*)chunks[chunk_index]) + chunk_offset);
    }

    const T& get(size_t index) const
    {
        auto chunk_index = index / count_per_chunk;
        auto chunk_offset = index % count_per_chunk;

        assert(chunk_index < chunks.size());

        return *(((const T*)chunks[chunk_index]) + chunk_offset);
    }

    template <class... Arguments>
    T& add(size_t index, Arguments&&... args)
    {
        auto chunk_index = index / count_per_chunk;
        if (chunk_index >= chunks.size())
            create_chunk(chunk_index);

        return *(new (&get(index)) T{ std::forward<Arguments>(args)... });
    }

    void remove(size_t index)
    {
        // TODO free chunk when last component is removed from it.
        (&get(index))->~T();
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

    world() = default;
    world(world<Components...>&&) = default;
    world<Components...>& operator=(world<Components...>&&) = default;

    entity create()
    {
        count_++;
        auto e = create_handle(entities, free_entities);
        if (e.index >= entity_masks.size())
            entity_masks.resize(e.index + 1);
        entity_masks[e.index] = 0;

        return e;
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
        return is_valid_handle(entities, free_entities, e);
    }

    template <class Component, class... Arguments>
    Component& add(entity e, Arguments&&... args)
    {
        static_assert(contains_type_v<Component, component_set_type>, "This world does not contain that component type.");

        assert(is_alive(e));
        assert(!has<Component>(e));

        entity_masks[e.index] |= type_mask_v<type_set_t<Component>, component_set_type>;

        return create<Component>(e, std::forward<Arguments>(args)...);
    }

    template <class Component>
    bool has(entity e) const
    {
        static_assert(contains_type_v<Component, component_set_type>, "This world does not contain that component type.");

        if (!is_alive(e))
            return false;

        auto comp_mask = type_mask_v<type_set_t<Component>, component_set_type>;

        return (entity_masks[e.index] & comp_mask) == comp_mask;
    }

    template <class Component>
    Component& get(entity e)
    {
        static_assert(contains_type_v<Component, component_set_type>, "This world does not contain that component type.");

        assert(is_alive(e));
        assert(has<Component>(e));
        auto& data = std::get<index_of_type_v<Component, component_set_type>>(component_data);
        return data.get(e.index);
    }

    template <class Component>
    const Component& get(entity e) const
    {
        static_assert(contains_type_v<Component, component_set_type>, "This world does not contain that component type.");

        assert(is_alive(e));
        assert(has<Component>(e));
        const auto& data = std::get<index_of_type_v<Component, component_set_type>>(component_data);
        return data.get(e.index);
    }

    template <class Component>
    void remove(entity e)
    {
        static_assert(contains_type_v<Component, component_set_type>, "This world does not contain that component type.");

        assert(is_alive(e));
        if (has<Component>(e)) {
            entity_masks[e.index] &= ~type_mask_v<type_set_t<Component>, component_set_type>;
            auto& data = std::get<index_of_type_v<Component, component_set_type>>(component_data);
            data.remove(e.index);
        }
    }

    void remove_components(entity e)
    {
        using expand_type = int[];
        expand_type d{ (world::template remove<Components>(e), 0)... };
        (void)d;
    }

    void destroy(entity e)
    {
        if (!is_alive(e))
            return;
        count_--;
        remove_components(e);
        destroy_handle(entities, free_entities, e);
    }

    template <class... SubComponents, class F>
    void for_each(F f)
    {
        static_assert(is_type_subset_v<type_set_t<SubComponents...>, component_set_type>, "Can only iterate over a subset of components is this world.");

        auto mask{ type_mask_v<type_set_t<SubComponents...>, component_set_type> };
        auto count{ entities.size() };

        for (std::size_t i = 0; i < count; ++i) {
            const auto& e = entities[i];
            if (is_alive(e) && ((entity_masks[e.index] & mask) == mask))
                f(e, std::get<index_of_type_v<SubComponents, component_set_type>>(component_data).get(e.index)...);
        }
    }

    template <class... SubComponents, class F>
    void for_each(F f) const
    {
        static_assert(is_type_subset_v<type_set_t<SubComponents...>, component_set_type>, "Can only iterate over a subset of components is this world.");

        auto mask{ type_mask_v<type_set_t<SubComponents...>, component_set_type> };
        auto count{ entities.size() };

        for (std::size_t i = 0; i < count; ++i) {
            const auto& e = entities[i];
            if (is_alive(e) && ((entity_masks[e.index] & mask) == mask))
                f(e, std::get<index_of_type_v<SubComponents, component_set_type>>(component_data).get(e.index)...);
        }
    }

    size_t size() const noexcept
    {
        return count_;
    }

private:
    world(const world<Components...>&) = delete;
    world<Components...>& operator=(const world<Components...>&) = delete;

    template <class Component, class... Arguments>
    Component& create(entity e, Arguments&&... args)
    {
        auto& data = std::get<index_of_type_v<Component, component_set_type>>(component_data);
        return data.add(e.index, std::forward<Arguments>(args)...);
    }

    std::size_t count_ = 0;
    std::vector<entity> entities;
    std::vector<std::uint32_t> free_entities;
    std::vector<component_mask_type> entity_masks;
    std::tuple<component_storage<Components>...> component_data;

    template <class... SubComponents>
    friend class world_view;
};

template <class... SubComponents>
class world_view {
public:
    using component_subset_type = component_set<SubComponents...>;

    template <class... Components>
    world_view(const world<Components...>& world)
        : count_{ world.count_ }
        , entities{ world.entities }
        , free_entities{ world.free_entities }
        , entity_masks{ world.entity_masks }
        , component_data{ std::get<index_of_type_v<SubComponents, component_set<Components...>>>(world.component_data)... }
        , maskes{ { (component_mask_type(1) << index_of_type_v<SubComponents, component_set<Components...>>)... } }
    {
        static_assert(is_type_subset_v<component_subset_type, component_set<Components...>>, "world_view components must be a subset of world components.");
    }

    bool is_alive(entity e) const noexcept
    {
        return is_valid_handle(entities, free_entities, e);
    }

    template <class Component>
    bool has(entity e) const
    {
        static_assert(contains_type_v<Component, component_subset_type>, "This world_view does not contain that component type.");

        if (!is_alive(e))
            return false;

        auto comp_mask = maskes[index_of_type_v<Component, component_subset_type>];

        return (entity_masks[e.index] & comp_mask) == comp_mask;
    }

    template <class Component>
    const Component& get(entity e) const
    {
        static_assert(contains_type_v<Component, component_subset_type>, "This world_view does not contain that component type.");

        assert(is_alive(e));
        assert(has<Component>(e));
        const auto& data = std::get<index_of_type_v<Component, component_subset_type>>(component_data);
        return data.get(e.index);
    }

    template <class... ForeachComponents, class F>
    void for_each(F f) const
    {
        static_assert(is_type_subset_v<type_set_t<ForeachComponents...>, component_subset_type>, "Can only iterate over a subset of components is this world_view.");

        auto mask = (maskes[index_of_type_v<ForeachComponents, component_subset_type>] | ...);
        auto count{ entities.size() };

        for (std::size_t i = 0; i < count; ++i) {
            const auto& e = entities[i];
            if (is_alive(e) && ((entity_masks[e.index] & mask) == mask))
                f(e, std::get<index_of_type_v<ForeachComponents, component_subset_type>>(component_data).get(e.index)...);
        }
    }

    size_t size() const noexcept
    {
        return count_;
    }

private:
    const std::size_t& count_;
    const std::vector<entity>& entities;
    const std::vector<std::uint32_t>& free_entities;
    const std::vector<component_mask_type>& entity_masks;
    std::tuple<const component_storage<SubComponents>&...> component_data;

    const std::array<component_mask_type, sizeof...(SubComponents)> maskes;
};
}

#endif // SIGMA_WORLD_HPP
