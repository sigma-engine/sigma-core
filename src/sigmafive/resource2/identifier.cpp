#include <sigmafive/resource2/identifier.hpp>

namespace sigmafive {
    namespace resource2 {

        identifier::identifier() {}

        identifier::identifier(const std::string &name) : name(std::move(name)) {
            // TODO add asserts that name is valid.
        }

        bool identifier::is_valid() const noexcept {
            return name.size() > 0;
        }

        bool identifier::is_subresource() const {
            return name.find(":") != name.npos;
        }

        std::string identifier::base() const {
            return name.substr(0,name.find(":"));
        }

        std::string identifier::sub() const {
            auto pos = name.find(":");
            if(pos == name.npos)
                return "";
            return name.substr(pos+1);
        }

    }
}
