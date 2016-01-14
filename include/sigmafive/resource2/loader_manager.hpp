#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/loader.hpp>
#include <sigmafive/resource2/identifier.hpp>

#include <list>
#include <memory>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API loader_manager : public loader {
        public:
            void register_loader(std::shared_ptr<loader> loader);

            bool can_load(identifier id);

            bool load(resource_manager &rm, identifier id, std::unique_ptr<std::istream> stream);

            void unregister_loader(std::shared_ptr<loader> loader);
        private:
            std::list<std::shared_ptr<loader>> loaders;
        };
    }
}
