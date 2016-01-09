#ifndef SIGMA_FIVE_RESOURCE_RESOURCE_CACHE_HPP
#define SIGMA_FIVE_RESOURCE_RESOURCE_CACHE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/resource/resource.hpp>

namespace sigmafive {
    namespace resource {
        class base_resource_cache {
        };

        template<class T>
        class resource_cache : public base_resource_cache {
        public:
        protected:
            friend class resource<T>;
            virtual T *acquire(identifier id) = 0;
            virtual const T *acquire(identifier id) const = 0;
        };
    }
}

#endif //SIGMA_FIVE_RESOURCE_RESOURCE_CACHE_HPP
