#include <sigmafive/resource2/filesystem_package.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <memory>
#include <fstream>

namespace sigmafive {
    namespace resource2 {
        bool filesystem_package::contains(identifier id) const {
            boost::filesystem::path path(id.base());
            return boost::filesystem::is_regular_file(path);
        }

        std::unique_ptr<std::istream> filesystem_package::get_stream(identifier id) {
            return std::make_unique<std::ifstream>(id.base());
        }
    }
}
