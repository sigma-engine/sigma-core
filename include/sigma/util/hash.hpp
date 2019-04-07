#ifndef SIGMA_ENGINE_HASH_HPP
#define SIGMA_ENGINE_HASH_HPP

#include <cstdint>
#include <functional>

namespace sigma {
namespace util {
    namespace detail {
        template <typename T>
        void hash_combine(size_t& seed, const T& val)
        {
            seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }

    // http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0814r0.pdf
    template <typename... Types>
    size_t hash_combine(const Types&... args)
    {
        size_t seed = 0;
        (detail::hash_combine(seed, args), ...);
        return seed;
    }

    // http://www.cse.yorku.ca/~oz/hash.html
    constexpr size_t compile_time_hash(const char* input)
    {
        return size_t(*input)
            ? static_cast<size_t>(*input) + size_t(33) * compile_time_hash(input + 1)
            : size_t(5381);
    }
}
}

#endif // SIGMA_ENGINE_HASH_HPP
