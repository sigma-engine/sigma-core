#ifndef SIGMA_COMPILER_UTIL_HPP
#define SIGMA_COMPILER_UTIL_HPP

#include <boost/filesystem/path.hpp>

namespace sigma {
void touch_stamp_file(boost::filesystem::path outputdir, boost::filesystem::path resource);

bool resource_has_changes(boost::filesystem::path outputdir, boost::filesystem::path resource);
}

#endif // SIGMA_COMPILER_UTIL_HPP
