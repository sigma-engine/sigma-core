#ifndef SIGMA_MATERIAL_COMPILER_HPP
#define SIGMA_MATERIAL_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_material(boost::filesystem::path file);

bool compile_materials(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> materials);
}

#endif // SIGMA_MATERIAL_COMPILER_HPP
