#ifndef SIGMAFIVE_SYSTEM_RESOURCE_HPP
#define SIGMAFIVE_SYSTEM_RESOURCE_HPP

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

namespace sigmafive {
	namespace system {
		class resource {
        public:
            virtual ~resource() = default;
        private:
            friend class boost::serialization::access;
            template <typename Archive>
            void serialize(Archive& ar, const unsigned int version) {
            }

            //resource(const resource &) = delete;
            //resource &operator=(const resource &) = delete;

            //resource(resource &&) = delete;
            //resource &operator=(resource &&) = delete;
		};
	}
}

#endif //SIGMAFIVE_SYSTEM_RESOURCE_HPP
