#ifndef SIGMA_CUBEMAP_COMPILER_HPP
#define SIGMA_CUBEMAP_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_cubemap(boost::filesystem::path file);

bool compile_cubemaps(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> cubemaps);
}

#endif // SIGMA_CUBEMAP_COMPILER_HPP
