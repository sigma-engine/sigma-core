#include <sigmafive/resource/resource.hpp>
#include <boost/uuid/random_generator.hpp>

namespace sigmafive {
    namespace resource {
        boost::uuids::uuid resource::generate_key() {
            static boost::uuids::random_generator generator;
            return generator();
        }

        resource::resource() : uuid_(generate_key()) {
        }

        boost::uuids::uuid resource::uuid() const {
            return uuid_;
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::resource::resource)
