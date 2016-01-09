#ifndef SIGMAFIVE_RESOURCE_RESOURCE_HPP
#define SIGMAFIVE_RESOURCE_RESOURCE_HPP

#include <sigmafive/config.hpp>

#include <string>
#include <limits>
#include <cstdint>

namespace sigmafive {
    namespace resource {
        struct SIGMAFIVE_API identifier {
            explicit identifier(std::uint64_t id = std::numeric_limits<std::uint64_t>::max()) noexcept;

            identifier(const std::string &name) noexcept;

            bool operator==(const identifier &o) const noexcept;

            bool operator!=(const identifier &o) const noexcept;

            bool is_valid() const noexcept;

            std::uint64_t value;
        };

        template<class>
        class resource_cache;

        template<class T>
        class resource { // TODO this is basically entity::component!
        public:
            resource(std::nullptr_t = nullptr) : cache(nullptr) {}

            explicit resource(identifier id,resource_cache<T> *cache) : id(id), cache(cache) {}

            bool operator==(const resource<T> &y) const noexcept {
                return cache == y.cache && id == y.id;
            }

            bool operator==(std::nullptr_t) const noexcept {
                return cache == nullptr || !id.is_valid() || get() == nullptr;
            }

            bool operator!=(const resource<T> &y) const {
                return !(*this == y);
            }

            bool operator!=(std::nullptr_t) const {
                return !(*this == nullptr);
            }

            T *operator->() {
                return cache->acquire(id);
            }

            const T *operator->() const {
                return cache->acquire(id);
            }

            T *get() {
                return (cache != nullptr) ? cache->acquire(id) : nullptr;
            }

            const T *get() const {
                return (cache != nullptr) ? cache->acquire(id) : nullptr;
            }

        private:
            identifier id;
            resource_cache<T> *cache;
        };

        template<class T>
        bool operator==(std::nullptr_t, const resource<T>& x) noexcept {
            return x == nullptr;
        }

        template<class T>
        bool operator!=(std::nullptr_t, const resource<T>& x) noexcept {
            return x != nullptr;
        }
    }
}

#endif //SIGMAFIVE_RESOURCE_RESOURCE_HPP
