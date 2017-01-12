#ifndef SIGMA_POST_PROCESS_EFFECT_COMPILER_HPP
#define SIGMA_POST_PROCESS_EFFECT_COMPILER_HPP

#include <boost/filesystem/path.hpp>

#include <vector>

namespace sigma {
bool is_post_process_effect(boost::filesystem::path file);

bool compile_post_process_effects(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> post_process_effects);
}

#endif // SIGMA_POST_PROCESS_EFFECT_COMPILER_HPP
