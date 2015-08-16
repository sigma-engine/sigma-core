#ifndef SIGMAFIVE_SYSTEM_RESOURCE_HPP
#define SIGMAFIVE_SYSTEM_RESOURCE_HPP

#include <object.hpp>

namespace sigmafive {
	namespace system {
        class resource : public object {
            SIGMAFIVE_OBJECT()
        public:
            resource() = default;

            resource(const resource &) = default;

            resource(resource &&) = default;

            resource& operator=(const resource &) = default;

            resource& operator=(resource &&) = default;

            virtual ~resource() = default;
        private:
		};
	}
}

#endif //SIGMAFIVE_SYSTEM_RESOURCE_HPP
