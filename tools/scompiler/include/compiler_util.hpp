#ifndef SIGMA_COMPILER_UTIL_HPP
#define SIGMA_COMPILER_UTIL_HPP

#include <boost/filesystem/path.hpp>

namespace sigma {
void touch_stamp_file(const boost::filesystem::path& outputdir, const boost::filesystem::path& resource, const std::vector<boost::filesystem::path>& dependencies = {});

bool resource_has_changes(const boost::filesystem::path& outputdir, const boost::filesystem::path& resource);
}

#endif // SIGMA_COMPILER_UTIL_HPP
