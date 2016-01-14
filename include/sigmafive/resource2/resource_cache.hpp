#ifndef SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP
#define SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP

#include <sigmafive/config.hpp>

#include <cppbr/cache/cache.hpp>
#include <cppbr/cache/handle.hpp>
#include <cppbr/cache/cache_factory.hpp>

#include <bitset>
#include <string>
#include <limits>
#include <cstdint>

namespace sigmafive {
    namespace resource2 {
        class identifier {
        public:
            identifier(const std::string &name);

            bool operator ==(const identifier &o) const noexcept {
                return value == o.value;
            }

            bool operator !=(const identifier &o) const noexcept {
                return value != o.value;
            }

            bool is_valid() const noexcept {
                return value.size() > 0;
            }
        private:
            std::string value;
        };

        class base_resource_cache {
        public:
            virtual bool contains(identifier id) const = 0;

            virtual bool add(identifier id) = 0;

            virtual bool remove(identifier id) = 0;
        private:
        };

        template<class T>
        using resource_cache = cppbr::cache::cache<identifier,T,base_resource_cache>;

        template<class T>
        using resource = typename resource_cache<T>::handle_type;

        using resource_cache_factory = cppbr::cache::cache_factory<identifier,std::bitset<64>,base_resource_cache>;
    }
}

#endif //SIGMA_FIVE_RESOURCE2_RESOURCE_CACHE_HPP
