#ifndef SIGMA_TEXTURE_COMPILER_HPP
#define SIGMA_TEXTURE_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_texture(boost::filesystem::path file);

bool compile_textures(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> textures);
}

#endif // SIGMA_TEXTURE_COMPILER_HPP
