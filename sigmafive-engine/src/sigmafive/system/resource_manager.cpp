#include <sigmafive/system/resource_manager.hpp>

#include <boost/uuid/uuid_generators.hpp>

namespace sigmafive {
	namespace system {
        resource_manager::resource_manager(boost::filesystem::path path)
            : path_(path) {
        }

        resource_manager::~resource_manager() {
        }

        boost::uuids::uuid resource_manager::generate_key() const {
            static boost::uuids::random_generator generator;
            return generator();
        }

        void resource_manager::insert(boost::uuids::uuid resource_key,boost::shared_ptr<resource> resource) {
            resources_[resource_key] = resource;
            //TODO write to file
        }

        boost::shared_ptr<resource> resource_manager::get(boost::uuids::uuid resource_key) {
            auto it = resources_.find(resource_key);
            if(it != resources_.end())
                return resources_[resource_key];
            //TODO read from file
            return nullptr;
        }
    }
}
