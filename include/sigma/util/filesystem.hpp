#ifndef SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP
#define SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP

#include <sigma/config.hpp>

#include <boost/filesystem/path.hpp>

namespace sigma {
namespace filesystem {
    bool SIGMA_API contains_file(boost::filesystem::path directory, boost::filesystem::path file);

    boost::filesystem::path SIGMA_API make_relative(boost::filesystem::path directory, boost::filesystem::path file);
}
}

#endif // SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP
