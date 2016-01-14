#pragma once

#include <sigmafive/config.hpp>

#include <memory>
#include <istream>

namespace sigmafive {
    namespace resource2 {
        class identifier;
        class resource_manager;

        class SIGMAFIVE_API loader {
        public:
            virtual ~loader() {}

            virtual bool can_load(identifier id) = 0;

            virtual bool load(resource_manager &rm,identifier id,std::unique_ptr<std::istream> stream) = 0;
        };
    }
}
