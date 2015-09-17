#ifndef SIGMAFIVE_RESOURCE_RESOURCE_HPP
#define SIGMAFIVE_RESOURCE_RESOURCE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <boost/uuid/uuid.hpp>

namespace sigmafive {
    namespace resource {
        class SIGMAFIVE_API resource : public object {
        SIGMAFIVE_CLASS()
        public:
            static boost::uuids::uuid generate_key();

            resource();

            virtual ~resource() = default;

            boost::uuids::uuid uuid() const;
        private:
            boost::uuids::uuid uuid_;
        };
    }
}

#endif //SIGMAFIVE_RESOURCE_RESOURCE_HPP
