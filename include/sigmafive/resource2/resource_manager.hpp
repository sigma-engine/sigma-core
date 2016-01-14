#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/cache.hpp>
#include <sigmafive/resource2/loader.hpp>
#include <sigmafive/resource2/package.hpp>

#include <assert.h>
#include <list>
#include <typeindex>
#include <unordered_map>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API resource_manager {
        public:
            resource_manager(package &package,loader &loader);

            template<class T>
            void register_cache(std::unique_ptr<resource_cache<T>> cache) {
                assert(caches.count(typeid(T)) == 0 && "resource cache already exits!");
                caches[typeid(T)] = std::move(cache);
            }

            template<class T>
            void insert(identifier id,T &&value) {
                cache_for<T>()->insert(id,std::forward<T>(value));
            }

            template<class T>
            resource<T> get(identifier id) {
                auto cache = cache_for<T>();
                auto r = cache->get(id);
                if(r == nullptr) {
                    assert(package_.contains(id) && "resource not found!");
                    assert(loader_.can_load(id)   && "resource not be loaded!");
                    loader_.load(*this,id,package_.get_stream(id));
                }
                return cache_for<T>()->get(id);
            }

            template<class T>
            void unregister_cache() {
                assert(caches.count(typeid(T)) == 1 && "resource cache does not exits!");
                caches.erase(typeid(T));
            }
        private:
            template<class T>
            resource_cache<T> *cache_for() {
                assert(caches.count(typeid(T)) == 1 && "resource cache does not exits!");
                return static_cast<resource_cache<T> *>(caches[typeid(T)].get());
            }
            package &package_;
            loader &loader_;
            std::unordered_map<std::type_index,std::unique_ptr<base_resource_cache>> caches;
        };
    }
}
