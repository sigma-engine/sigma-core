#ifndef SIGMAFIVE_SYSTEM_RESOURCE_MANAGER_HPP
#define SIGMAFIVE_SYSTEM_RESOURCE_MANAGER_HPP

#include <system/resource.hpp>

#include <unordered_map>
#include <boost/uuid/uuid.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/functional/hash.hpp>

namespace sigmafive {
	namespace system {
		class resource_manager {
        public:
            resource_manager(boost::filesystem::path = boost::filesystem::path("../data"));

            virtual ~resource_manager();

            resource_manager(const resource_manager &) = delete;

            resource_manager(resource_manager &&) = delete;

            resource_manager&operator=(const resource_manager &) = delete;

            resource_manager&operator=(resource_manager &&) = delete;

            boost::uuids::uuid generate_key() const;

            void insert(boost::uuids::uuid resource_key,boost::shared_ptr<resource> resource);

            boost::shared_ptr<resource> get(boost::uuids::uuid resource_key);

            template<class T>
            boost::shared_ptr<T> get(boost::uuids::uuid resource_key) {
                return boost::dynamic_pointer_cast<T>(get(resource_key));
            }
        private:
            boost::filesystem::path path_;
            std::unordered_map<boost::uuids::uuid,boost::shared_ptr<resource>,boost::hash<boost::uuids::uuid>> resources_;
        };
	}
}

#endif //SIGMAFIVE_SYSTEM_RESOURCE_MANAGER_HPP
