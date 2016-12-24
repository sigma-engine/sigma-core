#ifndef SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP
#define SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP

#include <cstdint>

namespace sigma {
namespace util {
    using hash_type = unsigned long long;

    // http://www.cse.yorku.ca/~oz/hash.html
    constexpr hash_type compile_time_hash(const char* input)
    {
        return hash_type(*input)
            ? static_cast<hash_type>(*input) + hash_type(33) * compile_time_hash(input + 1)
            : hash_type(5381);
    }
}
}

#endif // SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP
