#ifndef SIGMA_PACKAGE_SHADER_HPP
#define SIGMA_PACKAGE_SHADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_shaders(
    sigma::resource::database<sigma::graphics::shader>& shader_database,
    const boost::filesystem::path& source_directory,
    const std::vector<boost::filesystem::path>& include_directories);

#endif // SIGMA_PACKAGE_SHADER_HPP
