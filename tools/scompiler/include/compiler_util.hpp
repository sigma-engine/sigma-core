#ifndef SIGMA_COMPILER_UTIL_HPP
#define SIGMA_COMPILER_UTIL_HPP

#include <boost/filesystem/path.hpp>

namespace sigma {
void touch_stamp_file(const boost::filesystem::path& outputdir, const boost::filesystem::path& resource, const std::vector<boost::filesystem::path>& dependencies = {});
}

#endif // SIGMA_COMPILER_UTIL_HPP
