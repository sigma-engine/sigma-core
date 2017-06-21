#ifndef SIGMA_SHADER_COMPILER_HPP
#define SIGMA_SHADER_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_shader(boost::filesystem::path file);

bool compile_shaders(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> include_dirs, std::vector<boost::filesystem::path> shaders);
}

#endif // SIGMA_SHADER_COMPILER_HPP
