#include <sigmafive/resource2/loader_manager.hpp>

namespace sigmafive {
    namespace resource2 {

        void loader_manager::register_loader(std::shared_ptr<loader> loader) {
            loaders.push_front(loader);
        }

        bool loader_manager::can_load(identifier id) {
            for(auto loader:loaders) {
                if(loader->can_load(id))
                    return true;
            }
            return false;
        }

        bool loader_manager::load(resource_manager &rm, identifier id, std::unique_ptr<std::istream> stream) {
            for(auto loader:loaders) {
                if(loader->can_load(id))
                    return loader->load(rm,id,std::move(stream));
            }
            return false;
        }

        void loader_manager::unregister_loader(std::shared_ptr<loader> loader) {
            loaders.remove(loader);
        }

    }
}
