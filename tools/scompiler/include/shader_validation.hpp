#ifndef SIGMA_SHADER_VALIDATION_HPP
#define SIGMA_SHADER_VALIDATION_HPP

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace sigma {
namespace graphics {
    struct shader_data;
}
bool validate_shader(const graphics::shader_data& shader, std::vector<boost::filesystem::path> source_filenames);
}

#endif //SIGMA_SHADER_VALIDATION_HPP
