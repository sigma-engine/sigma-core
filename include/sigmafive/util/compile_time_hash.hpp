#ifndef SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP
#define SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP

#include <cstdint>

namespace sigmafive {
    namespace util {
        //http://www.cse.yorku.ca/~oz/hash.html
        constexpr std::size_t compile_time_hash(const char *input) {
            return std::size_t(*input) ? static_cast<std::size_t>(*input) + std::size_t(33) * compile_time_hash(input + 1) : std::size_t(5381);
        }
    }
}

#endif //SIGMA_FIVE_ENGINE_COMPILE_TIME_HASH_HPP
