#ifndef SIGMA_TYPE_SEQUENCE_HPP
#define SIGMA_TYPE_SEQUENCE_HPP

#include <cstdint>
#include <type_traits>

namespace sigma {
template <class... TypesSet>
struct type_set {
    template <class TagType>
    struct type_tag {
        using type = TagType;
    };

    template <class Func>
    static constexpr void for_each(Func f)
    {
        (f(type_tag<TypesSet>()), ...);
    }
};

// Count number of occurrences of `T` in `Types`.
template <class T, class... Types>
struct count_type {
    static constexpr const std::size_t value = (std::is_same_v<T, Types> + ...);
};

template <class T>
struct count_type<T> {
    static constexpr const std::size_t value = 0;
};

template <class T, class... Types>
inline constexpr const std::size_t count_type_v = count_type<T, Types...>::value;

// Check if `T` is in `Types`.
template <class T, class... Types>
struct contains_type {
    static constexpr const bool value = (std::is_same_v<T, Types> || ...);
};

template <class T>
struct contains_type<T> {
    static constexpr const bool value = false;
};

template <class T, class... Types>
inline constexpr const bool contains_type_v = contains_type<T, Types...>::value;

// Check if `Types` is a set.
template <class... Types>
struct is_type_set {
    static constexpr const bool value = (count_type_v<Types, Types...> + ...) == sizeof...(Types);
};

template <>
struct is_type_set<> {
    static constexpr const bool value = true;
};

template <class... Types>
inline constexpr const bool is_type_set_v = is_type_set<Types...>::value;

// Static assert if user tries to make a type_set that is not a set.
namespace detail {
    template <class... TypesSet>
    struct type_set_helper {
        static_assert(is_type_set_v<TypesSet...>, "Elements in a type_set must be unique.");
        using type = type_set<TypesSet...>;
    };
}

template <class... Types>
using type_set_t = typename detail::type_set_helper<Types...>::type;

template <class T, class... Types>
struct contains_type<T, type_set<Types...>> {
    static constexpr const bool value = contains_type<T, Types...>::value;
};

// Get the index of type `T` from the type_set `TypeSet`.
template <class T, class TypeSet>
struct index_of_type;

template <class T, class Head, class... Tail>
struct index_of_type<T, type_set<Head, Tail...>> {
    static_assert(contains_type_v<T, Head, Tail...>, "The type_set must contain the type to find it's index.");
    static constexpr const std::size_t value = 1 + index_of_type<T, type_set<Tail...>>::value;
};

template <class T, class... Tail>
struct index_of_type<T, type_set<T, Tail...>> {
    static constexpr const std::size_t value = 0;
};

template <class T, class TypeSet>
inline constexpr const std::size_t index_of_type_v = index_of_type<T, TypeSet>::value;

// Check if `TypeSubSet` is a subset of `TypeSet`.
template <class TypeSubSet, class TypeSet>
struct is_type_subset;

template <class... TypeSubSet, class... TypeSet>
struct is_type_subset<type_set<TypeSubSet...>, type_set<TypeSet...>> {
    static constexpr const bool value = (contains_type_v<TypeSubSet, TypeSet...> && ...);
};

template <class TypeSubSet, class TypeSet>
inline constexpr const bool is_type_subset_v = is_type_subset<TypeSubSet, TypeSet>::value;

// Generate a mask for the subset of `TypeSubSet` of types in `TypeSet`.
template <class TypeSubSet, class... TypeSet>
struct type_mask;

template <class... TypeSubSet, class... TypeSet>
struct type_mask<type_set<TypeSubSet...>, type_set<TypeSet...>> {
    static_assert(sizeof...(TypeSet) <= 64, "Can only create a type mask for 64 or less types.");
    static constexpr const std::uint64_t value = ((std::uint64_t(1) << index_of_type_v<TypeSubSet, type_set<TypeSet...>>) | ...);
};

template <class... TypeSet>
struct type_mask<type_set<>, type_set<TypeSet...>> {
    static constexpr const std::uint64_t value = 0;
};

template <class TypeSubSet, class TypeSet>
inline constexpr const std::uint64_t type_mask_v = type_mask<TypeSubSet, TypeSet>::value;
}

#endif // SIGMA_TYPE_SEQUENCE_HPP
