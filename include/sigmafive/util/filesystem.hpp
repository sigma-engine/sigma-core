#ifndef SIGMA_FIVE_UTIL_FILESYSTEM_HPP
#define SIGMA_FIVE_UTIL_FILESYSTEM_HPP

#include <sigmafive/config.hpp>

#include <boost/filesystem/path.hpp>

namespace sigmafive {
namespace util {
    /**
     * Returns true if file is in the directory
     * @param directory the directory to check in MUST be absolute.
     * @param file the file to check for MUST be absolute.
     * @return
     */
    bool SIGMAFIVE_API directory_contains_file(boost::filesystem::path directory, boost::filesystem::path file);

    boost::filesystem::path SIGMAFIVE_API path_divergence(boost::filesystem::path directory, boost::filesystem::path file);
}
}

#endif //SIGMA_FIVE_UTIL_FILESYSTEM_HPP
