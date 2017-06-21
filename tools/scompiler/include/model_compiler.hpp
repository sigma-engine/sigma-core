#ifndef SIGMA_MODEL_COMPILER_HPP
#define SIGMA_MODEL_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_model(boost::filesystem::path file);

bool compile_models(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> models);
}

#endif // SIGMA_MODEL_COMPILER_HPP
