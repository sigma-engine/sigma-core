#pragma once

#include <sigmafive/config.hpp>

#include <string>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API identifier {
        public:
            identifier();

            identifier(const std::string &name);

            bool is_valid() const noexcept;

            bool is_subresource() const;

            std::string base() const;

            std::string sub() const;
        private:
            std::string name;
        };
    }
}
