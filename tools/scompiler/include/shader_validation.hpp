#ifndef SIGMA_SHADER_VALIDATION_HPP
#define SIGMA_SHADER_VALIDATION_HPP

#include <string>
#include <vector>

namespace sigma {
namespace graphics {
    struct shader_data;
}
bool validate_shader(const graphics::shader_data& shader, std::vector<std::string> source_filenames);
}

#endif //SIGMA_SHADER_VALIDATION_HPP
