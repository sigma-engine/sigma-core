#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/identifier.hpp>

#include <memory>
#include <istream>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API package {
        public:
            virtual ~package() {}

            virtual bool contains(identifier id) const = 0;

            virtual std::unique_ptr<std::istream> get_stream(identifier id) = 0;
        };
    }
}
