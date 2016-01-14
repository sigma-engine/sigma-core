#include <sigmafive/resource2/resource_manager.hpp>

namespace sigmafive {
    namespace resource2 {
        resource_manager::resource_manager(package &package, loader &loader)
            : package_(package), loader_(loader) {
        }
    }
}
