#ifndef SIGMA_FIVE_UTIL_FILESYSTEM_HPP
#define SIGMA_FIVE_UTIL_FILESYSTEM_HPP

#include <sigma/config.hpp>

#include <boost/filesystem/path.hpp>

namespace sigma {
namespace util {
    /**
 * Returns true if file is in the directory
 * @param directory the directory to check in MUST be absolute.
 * @param file the file to check for MUST be absolute.
 * @return
 */
    bool sigma_API directory_contains_file(boost::filesystem::path directory,
        boost::filesystem::path file);

    boost::filesystem::path sigma_API
    path_divergence(boost::filesystem::path directory,
        boost::filesystem::path file);
}
}

#endif // SIGMA_FIVE_UTIL_FILESYSTEM_HPP
