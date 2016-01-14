#pragma once

#include <sigmafive/config.hpp>

#include <sigmafive/resource2/package.hpp>

#include <list>
#include <string>

namespace sigmafive {
    namespace resource2 {
        class SIGMAFIVE_API package_manager : public package {
        public:
            void add_package(std::string name,std::unique_ptr<package> pack);

            virtual bool contains(identifier id) const override;

            virtual std::unique_ptr<std::istream> get_stream(identifier id) override;

            void remove_package(std::string name);
        private:
            std::list<std::pair<std::string,std::unique_ptr<package>>> packages;
        };
    }
}
