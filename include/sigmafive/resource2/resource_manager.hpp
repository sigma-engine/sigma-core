#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/cache.hpp>
#include <sigmafive/resource2/package.hpp>

#include <assert.h>
#include <list>
#include <typeindex>
#include <unordered_map>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API resource_manager {
        public:
            resource_manager(package &package);
        private:
        };
    }
}
