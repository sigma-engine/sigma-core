#ifndef SIGMAFIVE_SYSTEM_RESOURCE_HPP
#define SIGMAFIVE_SYSTEM_RESOURCE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

namespace sigmafive {
    namespace system {
        class SIGMAFIVE_API resource : public object {
        SIGMAFIVE_CLASS()
        public:
            virtual ~resource() = default;

        private:
        };
    }
}

#endif //SIGMAFIVE_SYSTEM_RESOURCE_HPP
