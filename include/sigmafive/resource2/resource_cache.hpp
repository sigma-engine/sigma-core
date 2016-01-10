#ifndef SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP
#define SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP

#include <sigmafive/config.hpp>

#include <cppbr/cache/cache.hpp>
#include <cppbr/cache/handle.hpp>

#include <limits>

namespace sigmafive {
    namespace resource2 {
        class identifier {
        public:
            identifier(const std::string &name);

            explicit identifier(std::uint64_t value=std::limits<std::uint64_t>::max()) noexcept;

            bool operator ==(const identifier &o) const noexcept {
                return value == o.value;
            }

            bool operator !=(const identifier &o) const noexcept {
                return value != o.value;
            }

            bool is_valid() const noexcept {
                return value != std::limits<std::uint64_t>::max();
            }
        private:
            std::uint64_t value;
        };

        template<class T>
        using resource_cache = cppbr::cache::cache<identifier,T>; // TODO add a base cache?

        template<class T>
        using resource = resource_cache<T>::handle_type;
    }
}

#endif //SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP
