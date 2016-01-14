#include <sigmafive/resource2/package_manager.hpp>

#include <algorithm>

namespace sigmafive {
    namespace resource2 {

        void package_manager::add_package(std::string name, std::unique_ptr<package> pack) {
            packages.push_front(std::make_pair(name,std::move(pack))); // TODO what should happen if two packages have the same name!
        }

        bool package_manager::contains(identifier id) const {
            for(auto &package:packages) {
                if(package.second->contains(id))
                    return true;
            }
            return false;
        }

        std::unique_ptr<std::istream> package_manager::get_stream(identifier id) {
            for(auto &package:packages) {
                if(package.second->contains(id))
                    return package.second->get_stream(id);
            }
            return nullptr;
        }

        void package_manager::remove_package(std::string name) {
            auto it = std::remove_if(packages.begin(),packages.end(),[name](auto &package){
                return package.first == name;
            });
            packages.erase(it,packages.end()); // TODO this erases all packages with the name name.
        }
    }
}
